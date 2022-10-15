#include "help_func.h"

/* define size of set array */
int set_len = 0;
/* define my set as local_vars -- this is custom datatype found in header file */
local_vars *my_set;



/***
Function Description: this function like printf but it used system call function "write" to write in stdout
parameter: const char *buf : buffer of string 
return: int num:  number of words 
***/
void print(const char *buf)
{
    if (write(1, buf, strlen(buf)) == -1)
		fprintf(stderr, RED "Error in print");
}
/***
Function Description: this function like fprintf but it used system call function "write" to write in stderr
parameter: const char *buf : buffer of string 
return: int num:  number of words 
***/
void fprint(const char *buf)
{
    if (write(2, buf, strlen(buf)) == -1)
		fprintf(stderr, RED "Error in fprint");
}
/***
Function Description: this function count how many word in input sperated by space
parameter:  char str[]: input string  from user
return: int num:  number of words 
***/
int words_num(char str[])
{
    /* create copy of input str in heap memory*/
    char *str_copy = malloc((strlen(str)+1) * sizeof(char));
    strcpy(str_copy,str);

    char *ptr;        
    int num = 0;
    /* split input str by spaces and count each word in line */
    ptr = strtok(str_copy," ");
    while (ptr != NULL){
        if(ptr != NULL)
        {
            num++;
        }
        ptr = strtok(NULL," ");
    }
    num++; //end by NULL
    /* free heap memory */
    free(str_copy);
    /* return number of words in input line */
    return num;

}

/***
Function Description: this function pharising the input into words
parameter:  char str[]: input string from user, int num_of_words: in input string
return: char** split: array of sting contain each word and ended by NULL
***/
char** pharsing(char str[], int num_of_words)
{
    /* create copy of input str in heap memory*/
    char *str_copy = malloc((strlen(str)+1) * sizeof(char));
    strcpy(str_copy,str);

    /* create 2d array in heap memory for splited words */
    char **split;
    split = (char**) malloc(num_of_words * sizeof(char*));

    /* split input str by spaces and save each word in split array */
    char *ptr = strtok(str_copy," ");
    for(int i=0; i< (num_of_words-1); i++)
    {   
        split[i] = (char*) malloc((strlen(ptr)+1) * sizeof(char));
        strcpy(split[i],ptr);
        ptr = strtok(NULL," ");
    }
    /* end split array by NULL --used as argv arr-- */
    split[num_of_words-1] = (char*) malloc(sizeof(void *));
    split[num_of_words-1] = NULL;    
    /* free heap memory from copy of input str */
    free(str_copy);
    /* return split arr pointer -- this is array in heap memory now */
    return split;
}

void redirectoin_func(char *const argv[])
{
    /* under dovelopment func to handle IO redirection */
}

/***
Function Description: this function creat new process by fork & execvp funcs.
parameter:  const char *file, char *const argv[] : command file name like ls, cd , ps,... | argument of command 
            redirection_process redirc_flag: flag to check if found IO redirection or not
return: int : -1 in falid otherwise return 0
***/
int create_process(const char *file, char *const argv[],redirection_process redirc_flag)
{
    /* create a child process */
    int pid = fork();
    /* check if in child process */
    if(pid == 0)
    {   if(redirc_flag)
            redirectoin_func(argv);
        /* replaces the current process image with a new process image.*/
        execvp(file,argv);
        /* if return then print error massege and return -1 to end process*/
        fprint(RED "Error in create process exec \n" RESET);
        return -1;
    }
    /* check if faild to create child process*/
    else if (pid == -1)
    {
        fprint(RED "Error in create process fork \n" RESET);
        return -1;
    }
    /* else my main code --parent process now-- wait my child to terminade and contiune */
    else
        if(wait(&pid) == -1)
        {
            fprint(RED "Error in create process wait child process \n" RESET);
            return -1;
        }
    return 0;
}
/***
Function Description: this function add new local varible to my_set array
parameter:  char str[]: input string from user like to name=value ex x=5, y=ahmed
return: none
***/
void add_local_varible(char str[])
{
    /* split line into name and value by '=' char*/
    char * name = strtok(str,"=");
    char * value = strtok(NULL,"=");

    /*check if the name of local varible defined before to overwite its value and retrun the function --end func--*/
    for(int i=0; i<set_len; i++)
    {
        if(strcmp(my_set[i].var_name,name) == 0)
        {
            strcpy(my_set[i].var_value,value);
            return;
        }
    }
    /* if new varible is defined */
    /* 
     * check if the varible is first in set to create set 
     * or not to just realloc its size 
     */
    if(set_len == 0)
        my_set = (local_vars *) malloc( sizeof(local_vars));
    else
        my_set = (local_vars *) realloc(my_set,(set_len + 1) * sizeof(local_vars));
    /* define name and value of local varible */
    my_set[set_len].var_name  = (char*) malloc((strlen(name)+1) * sizeof(char));
    strcpy(my_set[set_len].var_name, name);

    my_set[set_len].var_value = (char*) malloc((strlen(value)+1) * sizeof(char));
    strcpy(my_set[set_len].var_value, value);
    /* increase size of set array by 1 */
    set_len++;
}
/***
Function Description: this function print all local varible in my_set array
parameter:  none
return: none
***/
void get_local_varible()
{
    for(int i=0; i<set_len; i++)
        printf("local_vars[%d]: %s = %s\n", i, my_set[i].var_name, my_set[i].var_value);
}
/***
Function Description: this function search for varible in local varible (my_set array) and mov it to enviroment varibles
parameter:  char name[]: varible name
return: none
***/
void mv_local_to_env(char name[])
{
    for(int i=0; i<set_len; i++)
    {
        if(strcmp(my_set[i].var_name,name) == 0)
        {
            setenv(my_set[i].var_name,my_set[i].var_value,1);
            return;
        }
    }
    /* if not found the varible name in local varible (my_set array) print error massege*/
    fprint(RED "Error " GRN);
    fprint(name);
    fprint(RED " not set\n" RESET);
}
/***
Function Description: this function exit the shell and free the heap memory
parameter:  char **split, int split_size : array of input sting contain each word | size of array (number of words)
return: none
***/
void my_exit(char **split, int split_size)
{
    /* free all the heap memory --split array & my_set array-- */
    for (int i = 0; i <= split_size; i++)
        free(split[i]);
    free(my_set);
    print(MAG "Good Bye :)\n");
}
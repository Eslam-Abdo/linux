#include "help_func.h"

int set_len = 0;
local_vars *my_set;



/***
Function Description: this function like printf but it used system call function write
parameter: const void *buf, size_t count : buffer of string and its size
return: int num:  number of words 
***/
void print(const char *buf)
{
    if (write(1, buf, strlen(buf)) == -1)
		printf(RED "Error in print");
}
/***
Function Description: this function count how many word in input sperated by space
parameter:  char str[]: input string  from user
return: int num:  number of words 
***/
int words_num(char str[])
{
    char *str_copy = malloc((strlen(str)+1) * sizeof(char));
    strcpy(str_copy,str);

    char *ptr;        
    int num = 0;

    ptr = strtok(str_copy," ");

    while (ptr != NULL){
        if(ptr != NULL)
        {
            // printf("%s\n",ptr);
            num++;
        }
        ptr = strtok(NULL," ");
    }
    num++; //end by NULL
    // printf("num of words = %d\n", num);
    free(str_copy);

    return num;

}
/***
Function Description: this function pharising the input into words
parameter:  char str[]: input string from user, int num_of_words: in input string
return: char** split: array of sting contain each word and ended by NULL
***/
char** pharsing(char str[], int num_of_words)
{
    char *str_copy = malloc((strlen(str)+1) * sizeof(char));
    strcpy(str_copy,str);


    char **split;
    split = (char**) malloc(num_of_words * sizeof(char*));

    char *ptr = strtok(str_copy," ");

    for(int i=0; i< (num_of_words-1); i++)
    {   
        split[i] = (char*) malloc((strlen(ptr)+1) * sizeof(char));
        strcpy(split[i],ptr);
        // printf("%s\n",split[i]);
        ptr = strtok(NULL," ");
    }
    split[num_of_words-1] = (char*) malloc(sizeof(void *));
    split[num_of_words-1] = NULL;    

    // printf("================\n");
    free(str_copy);
    return split;
}
/***
Function Description: this function creat new process by fork & execvp funcs.
parameter:  const char *file, char *const argv[] : command file name like ls, cd , ps,... | argument of command 
return: int : -1 in falid otherwise return 0
***/
int create_process(const char *file, char *const argv[])
{
    int pid = fork();
    if(pid == 0)
    {
        execvp(file,argv);
        print(RED "Error in create process exec \n" RESET);
        return -1;
    }
    else if (pid == -1)
    {
        print(RED "Error in create process fork \n" RESET);
        return -1;
    }
    else
        if(wait(&pid) == -1)
        {
            print(RED "Error in create process wait child process \n" RESET);
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
    char * name = strtok(str,"=");
    char * value = strtok(NULL,"=");

    for(int i=0; i<set_len; i++)
    {
        if(strcmp(my_set[i].var_name,name) == 0)
        {
            strcpy(my_set[i].var_value,value);
            return;
        }
    }

    if(set_len == 0)
        my_set = (local_vars *) malloc( sizeof(local_vars));
    else
        my_set = (local_vars *) realloc(my_set,(set_len + 1) * sizeof(local_vars));

    my_set[set_len].var_name  = (char*) malloc((strlen(name)+1) * sizeof(char));
    strcpy(my_set[set_len].var_name, name);

    my_set[set_len].var_value = (char*) malloc((strlen(value)+1) * sizeof(char));
    strcpy(my_set[set_len].var_value, value);
    
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
    print(RED "Error " GRN);
    print(name);
    print(RED " not set\n" RESET);
}
/***
Function Description: this function exit the shell and free the heap memory
parameter:  char **split, int split_size : array of input sting contain each word | size of array (number of words)
return: none
***/
void my_exit(char **split, int split_size)
{
    for (int i = 0; i <= split_size; i++)
        free(split[i]);
    free(my_set);
    print(MAG "Good Bye :)\n");
}
#ifndef _HELP_FUNC_H_
#define _HELP_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>



/* color of text */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/* define enum for redirction IO redirection*/
typedef enum {no_redic, redic}redirection_process;

/* define struct for local varible*/
typedef struct 
{
    char * var_name;
    char * var_value;
}local_vars;


/**************************** define of help funcs *****************************/
/***
Function Description: this function like printf but it used system call function write
parameter: const void *buf, size_t count : buffer of string and its size
return: int num:  number of words 
***/
void print(const char *buf);
/***
Function Description: this function like fprintf but it used system call function "write" to write in stderr
parameter: const char *buf : buffer of string 
return: int num:  number of words 
***/
void fprint(const char *buf);
/***
Function Description: this function count how many word in input sperated by space
parameter:  char str[]: input string  from user
return: int num:  number of words 
***/
int words_num(char str[]);
/***
Function Description: this function pharising the input into words
parameter:  char str[]: input string from user, int num_of_words: in input string
return: char** split: array of sting contain each word and ended by NULL
***/
char** pharsing(char str[], int num_of_words);

void redirectoin_func(char *const argv[]);
/***
Function Description: this function creat new process by fork & execvp funcs.
parameter:  const char *file, char *const argv[] : command file name like ls, cd , ps,... | argument of command 
return: int : -1 in falid otherwise return 0
***/
int create_process(const char *file, char *const argv[], redirection_process redirc_flag);
/***
Function Description: this function add new local varible to my_set array
parameter:  char str[]: input string from user like to name=value ex x=5, y=ahmed
return: none
***/
void add_local_varible(char str[]);
/***
Function Description: this function print all local varible in my_set array
parameter:  none
return: none
***/
void get_local_varible(void);
/***
Function Description: this function search for varible in local varible (my_set array) and mov it to enviroment varibles
parameter:  char name[]: varible name
return: none
***/
void mv_local_to_env(char name[]);
/***
Function Description: this function exit the shell and free the heap memory
parameter:  char **split, int split_size : array of input sting contain each word | size of array (number of words)
return: none
***/
void my_exit(char **split, int split_size);


#endif
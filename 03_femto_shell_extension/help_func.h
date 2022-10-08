#ifndef _HELP_FUNC_H_
#define _HELP_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


#define MAX_LIMIT	100
/* color of text */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct 
{
    char * var_name;
    char * var_value;
}local_vars;



void print(const char *buf);

int words_num(char str[]);
char** pharsing(char str[], int num_of_words);

int create_process(const char *file, char *const argv[]);

void add_local_varible(char str[]);
void get_local_varible(void);
void mv_local_to_env(char name[]);

void my_exit(char **split, int split_size);


#endif
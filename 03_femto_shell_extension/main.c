#include <stdio.h>
#include <string.h>
#include "help_func.h"

#define MAX_LIMIT	100

int main()
{
    char input_str[MAX_LIMIT];
    while (1) 
    {
        /* get input from user */
        print(YEL "enter command:~$ " RESET);
        fgets(input_str, MAX_LIMIT, stdin);
        input_str[strlen(input_str)-1] = '\0';

        /* pharsing input to split_str varible (dynamic 2d array)*/
        int num_of_words = words_num(input_str);
        char** split_str = pharsing(input_str,num_of_words);
        
        /* check if input not empty */
        if(split_str[0] != NULL)
        {
            /* built in exit command */
            if (strcmp(split_str[0], "exit") == 0)
            {
                my_exit(split_str,num_of_words);
                return 0;
            }
            /* built in export command */
            else if (strcmp(split_str[0], "export") == 0)
                mv_local_to_env(split_str[1]);
            /* built in set command */
            else if (strcmp(split_str[0], "set") == 0)
                get_local_varible();
            /* check in input is local varible */
            else if (strchr(split_str[0],'=')!= NULL)
                add_local_varible(split_str[0]);
            /* if something else create new process using fork & execvp funcs */
            else 
                if(create_process(split_str[0],split_str)==-1)
                    return -1;
        }
        /* free heap memory and loop again*/
        free(split_str);
    }

    return 0;
}

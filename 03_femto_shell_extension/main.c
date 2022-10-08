#include <stdio.h>
#include <string.h>

#include "help_func.h"



int main()
{
    char str[MAX_LIMIT];
    while (1) 
    {
        print(YEL "enter command:~$ " RESET);
        fgets(str, MAX_LIMIT, stdin);
        str[strlen(str)-1] = '\0';

        int num_of_words = words_num(str);
        char** split_str = pharsing(str,num_of_words);
        
        if(split_str[0] != NULL)
        {
            if (strcmp(split_str[0], "exit") == 0)
            {
                my_exit(split_str,num_of_words);
                return 0;
            }
            else if (strcmp(split_str[0], "export") == 0)
                mv_local_to_env(split_str[1]);
            else if (strcmp(split_str[0], "set") == 0)
                get_local_varible();
            else if (strchr(split_str[0],'=')!= NULL)
                add_local_varible(split_str[0]);
            else 
                if(create_process(split_str[0],split_str)==-1)
                    return -1;
        }
        free(split_str);
    }

    return 0;
}

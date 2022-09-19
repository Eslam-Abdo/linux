#include <stdio.h>
#include <string.h>

#define MAX_LIMIT	100
int main()
{
    char str[MAX_LIMIT];
    while (1) {
	printf("enter command:~$ ");
	fgets(str, MAX_LIMIT, stdin);
	if (strcmp(str, "exit\n") == 0)
	    break;
	printf("%s", str);
    }
    printf("Good Bye :)\n");

    return 0;
}


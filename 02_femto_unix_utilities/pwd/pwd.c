#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



void print(const void *buf, size_t count)
{
	if( write(1, buf, count) == -1)
		printf("Error in print");
}

void pwd()
{
    char buf[100];
    char *buff;
    
	buff = getcwd(buf, 100);
    
	if (buff == NULL)
		printf("Error pwd!");
    
	print("pwd:\n", 6);
    print(buf, strlen(buf));
    print("\n", 1);
}


int main(int argc, char *argv[])
{
    pwd();
	
    return 0;
}


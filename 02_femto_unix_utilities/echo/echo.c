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

int main(int argc, char *argv[])
{

	for (int i = 1; i < argc; i++) {
		print(argv[i], strlen(argv[i]));
		print(" ",1);
	}
		print("\n",1);
		
    return 0;
}


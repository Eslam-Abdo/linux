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

void move_file(const char* src_pathname, const char* dest_pathname)
{
	if(rename(src_pathname, dest_pathname)==-1)
		print("Error! move file\n",18);
	print("sucess finish move\n",20);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
		print("Error! please enter two argument only for mv\n",46);
	else
		move_file(argv[1],argv[2]);
		

    return 0;
}


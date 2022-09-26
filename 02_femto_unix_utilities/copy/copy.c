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


void copy_file(const char* src_pathname, const char* dest_pathname)
{
	int src_fd = open(src_pathname,O_RDONLY,0644);
	int dest_fd = open(dest_pathname, O_WRONLY|O_CREAT, 0644);
	/* check correct open */
	if(src_fd == -1)
		print("Error! open src file\n",22);
	if(dest_fd == -1)
		print("Error! open|creat dest file\n",29);
	
	char buff[100];
	int word_size;
	int write_check;
	/* copy file */
	do{
		word_size = read(src_fd,buff,100);
		if(word_size > 0)
			write_check = write(dest_fd,buff,word_size);
		
	}while(word_size > 0 && write_check != -1);
	
	/* check correct copy */
	if(word_size == 0)
		print("sucess finish copy\n",20);
	else if(word_size == -1)
		print("Error! read src file\n",22);
	if(write_check == -1)
		print("Error! write in dest file\n",27);
	
	/* check correct close */
	if(close(src_fd) == -1)
                print("Error! close src file\n",23);
        if(close(dest_fd) == -1)
                print("Error! close dest file\n",24);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
		print("Error! please enter two argument for cp only\n",46);
	else
		copy_file(argv[1],argv[2]);


    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



void print(const void *buf, size_t count)
{
    if (write(1, buf, count) == -1)
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

void copy_file(const char *src_pathname, const char *dest_pathname)
{
    int src_fd = open(src_pathname, O_RDONLY, 0644);
    int dest_fd = open(dest_pathname, O_WRONLY | O_CREAT, 0644);
    /* check correct open */
    if (src_fd == -1)
		print("Error! open src file\n", 22);
    if (dest_fd == -1)
		print("Error! open|creat dest file\n", 29);

    char buff[100];
    int word_size;
    int write_check;
    /* copy file */
    do {
		word_size = read(src_fd, buff, 100);
		if (word_size > 0)
			write_check = write(dest_fd, buff, word_size);

    } while (word_size > 0 && write_check != -1);

    /* check correct copy */
    if (word_size == 0)
		print("sucess finish copy\n", 20);
    else if (word_size == -1)
		print("Error! read src file\n", 22);
    if (write_check == -1)
		print("Error! write in dest file\n", 27);

    /* check correct close */
    if (close(src_fd) == -1)
		print("Error! close src file\n", 23);
    if (close(dest_fd) == -1)
		print("Error! close dest file\n", 24);
}


void move_file(const char *src_pathname, const char *dest_pathname)
{
    if (rename(src_pathname, dest_pathname) == -1)
		print("Error! move file\n", 18);
    print("sucess finish move\n", 20);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
		print("Error! please enter your command\n", 34);
    } else {
		if (strcmp(argv[1], "pwd") == 0)
			pwd();
		else if (strcmp(argv[1], "echo") == 0) {
			for (int i = 2; i < argc; i++) {
				print(argv[i], strlen(argv[i]));
				print(" ", 1);
			}
			print("\n", 1);
		} else if (strcmp(argv[1], "cp") == 0) {
			if (argc != 4)
				print("Error! please enter two argument for cp only\n",46);
			else
				copy_file(argv[2], argv[3]);
		} else if (strcmp(argv[1], "mv") == 0) {
			if (argc != 4)
				print("Error! please enter two argument only for mv\n",46);
			else
				move_file(argv[2], argv[3]);
		}else
			print("wrong command try again\n", 30);
    }
    return 0;
}

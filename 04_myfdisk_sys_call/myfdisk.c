#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>


#include "MBR.h"
#include "GPT.h"
#include "myfdisk.h"


/***
Function Description: this function read partoin table (GPT & MBR) like "fdisk -l" program 
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
return:  none
***/
void myfdisk(const char *partion)
{
    int fd = open(partion,O_RDONLY,0644);
    /* check correct open */
	if(fd == -1)
    {
		printf("Error! open partion file\n");
        return;
    }
    char buff[SECTOR_SIZE];
    /* check correct read */
    if(read(fd,buff,SECTOR_SIZE) == -1)
    {
		printf("Error! read partion file LBA 0\n");
        return;
    }
    uint8_t type = buff[450];
    /* check for table type GPT or MBR */
    if(type == 0xee)
        /* print GPT table */
        GPT_partionTable(partion,fd);
    else
        /* print MBR table */
        MBR_partionTable(partion, fd, (dos_PartionEntry*)&buff[446]);
    /* check correct close */
	if(close(fd) == -1)
		printf("Error! close partion file\n");
}

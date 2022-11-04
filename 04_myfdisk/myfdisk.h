#ifndef _MYFDISK_H_
#define _MYFDISK_H_


#define SECTOR_SIZE         512

/* color of text */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/**************************** define ofs funcs *****************************/

/***
Function Description: this function read partoin table (GPT & MBR) like "fdisk -l" program 
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
return:  none
***/
void myfdisk(const char *partion);

#endif 
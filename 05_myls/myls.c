#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <inttypes.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/ioctl.h>

#include "myls.h"



/***
Function Description: this function return type of file
parameter:  uint16_t type : file type
return:  char of type
***/
char file_type(uint16_t type)
{
    switch (type)
    {
    case DT_BLK:    return 'b';
    case DT_CHR:    return 'c';
    case DT_DIR:    return 'd';
    case DT_LNK:    return 'l';
    case DT_REG:    return 'r';
    default:
        return '-';
    }
}
/***
Function Description: this function print file type and its mode (permission)
parameter:  mode_t stmode: mode from stat struct
return:  none
***/
void file_mode(mode_t stmode)
{
    mode_u fmode;
    fmode.varible = stmode;
    printf("%c%c%c%c%c%c%c%c%c%c ",
            file_type(fmode.fileType),
            (fmode.owner_r == 1)? 'r' : '-',
            (fmode.owner_w == 1)? 'w' : '-',
            (fmode.owner_x == 1)? 'x' : '-',
            (fmode.group_r == 1)? 'r' : '-',
            (fmode.group_w == 1)? 'w' : '-',
            (fmode.group_x == 1)? 'x' : '-',
            (fmode.other_r == 1)? 'r' : '-',
            (fmode.other_w == 1)? 'w' : '-',
            (fmode.other_x == 1)? 'x' : '-'
            );
}
/***
Function Description: this function return username of file from it's id
parameter:  uint16_t id: user id
return:  string : username
***/
char * uid_name(uint16_t id)
{
    struct passwd * uid_ptr = getpwuid(id);
    return uid_ptr->pw_name;
}
/***
Function Description: this function return group of file from it's id
parameter:  uint16_t id: group id
return:  string : group name
***/
char * gid_name(uint16_t id)
{
    struct group * gid_ptr = getgrgid(id);
    return gid_ptr->gr_name;
}
/***
Function Description: this function print header of ls 
parameter: none
return: none
***/
void print_header()
{
    /* inode    mode        links          uid          gid   Size                        time         name */
    printf(CYN"%-8s %-11s %-7s %-12s %-12s %-8s %-26s %s\n"RESET,
            "inode",
            "mode",
            "links",
            "uid",
            "gid",
            "Size",
            "time",
            "name"
            );
}
/***
Function Description: this function print file stat 
parameter: const char *dir_name: name of main dirctory
             const char *d_name: file name
return: none
***/ 
void print_file(const char *dir_name, const char *d_name)
{
    char * str;
    str = (char *)malloc(strlen(dir_name)+50*sizeof(char));
    strcpy(str,dir_name);
    if(str[strlen(str)-1] != '/')
        strcat(str,"/");
    strcat(str,d_name);

    struct stat mystat;

    if(stat(str, &mystat)==-1)
    {
        printf("Error! stat failed for %s\n",d_name);
        return;
    }
    char *lastModify_time = ctime(&mystat.st_mtime);
    lastModify_time[strlen(lastModify_time) - 2] = '\0';

    printf("%-8ld ",mystat.st_ino);
    file_mode(mystat.st_mode);
    printf("%6ld %12s %12s %6ld %27s %3c%s\n",
            mystat.st_nlink,
            uid_name(mystat.st_uid),
            gid_name(mystat.st_gid),
            mystat.st_size,
            lastModify_time,
            ' ',
            d_name
            );     
    free(str);
}
/***
Function Description: this function return number of coulom in terminal window
                        used to hadle ouput of printf
parameter: none
return: number of coulom in terminal window
***/
int get_windowSize()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    // printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);
    return w.ws_col;
}
/***
Function Description: this function print all file in dirctory like (ls with 3 flags -l, -a, -R) 
parameter: const char *dir_name: name of dirctory, 
            flag_t flags: struct contain 3 flags (-l, -a, -R) 
return: none
***/
void myls(const char *dir_name, flag_t flags)
{
    DIR * dir_ptr = opendir(dir_name);
    /* check correct open */
	if(dir_ptr == NULL)
    {
		printf("Error! open directory\n");
        return;
    }
    /* check if -R print dirctory name*/
    if(flags.recursive == 1)
        printf(YEL"%s:\n"RESET,dir_name);
    /* check if -l print header of ls */
    if(flags.list_files == 1)
        print_header();
    /*  number of coulom in terminal window */
    int wigth = get_windowSize();

    struct dirent* entry;
    while((entry = readdir(dir_ptr)) != NULL)
    {
        /* check if -a print hidden files */
        if(entry->d_name[0] == '.' && flags.all_files == 0)
            continue;
        /* check if -l print stat of file */
        if(flags.list_files == 1)
            print_file(dir_name,entry->d_name);
        else
        {
            /* method to handle output in different terminal window size*/
            wigth -= strlen(entry->d_name) - 4;
            if(wigth < strlen(entry->d_name))
            {
                printf("\n");
                wigth = get_windowSize();
                wigth -= strlen(entry->d_name) - 4;
            }
            /* just print name of files */
            printf(BLU"%s %3c"RESET,entry->d_name,' ');
        }
    }

    printf("\n");
    /* check if -R print each dirctory recursivly */
    if(flags.recursive == 1)
    {
        /* reset directory stream */
        rewinddir(dir_ptr);
        /* read each entry and check its type if it dicrtory file */
        while((entry = readdir(dir_ptr)) != NULL)
            if(entry->d_type == DT_DIR)
            {
                /* check if -a print hidden files */
                if(entry->d_name[0] == '.' && flags.all_files == 0)
                    continue;
                /* escape this to pointer in recursive */
                if((strcmp(entry->d_name,".")==0) || (strcmp(entry->d_name,"..")==0))
                    continue;
                /* add dirctory name with main dictory to str varible */
                char * str;
                str = (char *)malloc(strlen(dir_name)+50*sizeof(char));
                strcpy(str,dir_name);
                if(str[strlen(str)-1] != '/')
                    strcat(str,"/");
                strcat(str,entry->d_name);
                /* path str using recution method */
                myls(str,flags);
                /* free str pointer */
                free(str);
            }
    }
    /* check correct close */
	if(closedir(dir_ptr) == -1)
		printf("Error! close directory\n");
}

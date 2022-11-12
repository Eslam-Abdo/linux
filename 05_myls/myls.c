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
char * uid_name(uint16_t id)
{
    struct passwd * uid_ptr = getpwuid(id);
    return uid_ptr->pw_name;
}
char * gid_name(uint16_t id)
{
    struct group * gid_ptr = getgrgid(id);
    return gid_ptr->gr_name;
}
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
int get_windowSize()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    // printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);
    return w.ws_col;
}
void myls(const char *dir_name, flag_t flags)
{
    DIR * dir_ptr = opendir(dir_name);
    /* check correct open */
	if(dir_ptr == NULL)
    {
		printf("Error! open directory\n");
        return;
    }

    if(flags.recursive == 1)
        printf(YEL"%s:\n"RESET,dir_name);

    if(flags.list_files == 1)
        print_header();

    int wigth = get_windowSize();
    // printf("==%d\n",wigth);
    // printf("+ %5c",' ');
    // for(int i=1; i<= wigth; i++)
    //     printf("+");
    // printf("\n");
    struct dirent* entry;
    while((entry = readdir(dir_ptr)) != NULL)
    {
        if(entry->d_name[0] == '.' && flags.all_files == 0)
            continue;
        if(flags.list_files == 1)
            print_file(dir_name,entry->d_name);
        else
        {
            wigth -= strlen(entry->d_name) - 4;
            if(wigth < strlen(entry->d_name))
            {
                printf("\n");
                wigth = get_windowSize();
                wigth -= strlen(entry->d_name) - 4;
            }
            // printf("%d",wigth);
            printf(BLU"%s %3c"RESET,entry->d_name,' ');
        }
    }

    printf("\n");
    if(flags.recursive == 1)
    {
        rewinddir(dir_ptr);
        while((entry = readdir(dir_ptr)) != NULL)
            if(entry->d_type == DT_DIR)
            {
                if(entry->d_name[0] == '.' && flags.all_files == 0)
                    continue;
                if((strcmp(entry->d_name,".")==0) || (strcmp(entry->d_name,"..")==0))
                    continue;
                
                char * str;
                str = (char *)malloc(strlen(dir_name)+50*sizeof(char));
                strcpy(str,dir_name);
                if(str[strlen(str)-1] != '/')
                    strcat(str,"/");
                strcat(str,entry->d_name);

                myls(str,flags);
                // printf("\n");
                free(str);
            }
    }
    /* check correct close */
	if(closedir(dir_ptr) == -1)
		printf("Error! close directory\n");

    // printf("\n");
}

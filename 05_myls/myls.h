#include <sys/types.h>
#ifndef _MYLS_H_
#define _MYLS_H_

/* color of text */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/* define union for file mode */
typedef union
{
    /* define struct for 12-bits of file type and its mode */
    struct
    {
        uint16_t other_x:1;
        uint16_t other_w:1;
        uint16_t other_r:1;
        uint16_t group_x:1;
        uint16_t group_w:1;
        uint16_t group_r:1;
        uint16_t owner_x:1;
        uint16_t owner_w:1;
        uint16_t owner_r:1;
        uint16_t sticky:1;
        uint16_t sgid:1;
        uint16_t suid:1;
        uint16_t fileType:4;
    };
    uint16_t varible;
}mode_u;

/* define struct for ls flags (-l, -a, -R) */
typedef struct 
{
    uint8_t list_files;
    uint8_t all_files;
    uint8_t recursive;
}flag_t;


/**************************** define ofs funcs *****************************/
/***
Function Description: this function return type of file
parameter:  uint16_t type : file type
return:  char of type
***/
char file_type(uint16_t type);
/***
Function Description: this function print file type and its mode (permission)
parameter:  mode_t stmode: mode from stat struct
return:  none
***/
void file_mode(mode_t stmode);
/***
Function Description: this function return username of file from it's id
parameter:  uint16_t id: user id
return:  string : username
***/
char * uid_name(uint16_t id);
/***
Function Description: this function return group of file from it's id
parameter:  uint16_t id: group id
return:  string : group name
***/
char * gid_name(uint16_t id);
/***
Function Description: this function print header of ls 
parameter: none
return: none
***/
void print_header();
/***
Function Description: this function print file stat 
parameter: const char *dir_name: name of main dirctory
             const char *d_name: file name
return: none
***/ 
void print_file(const char *dir_name, const char *d_name);
/***
Function Description: this function return number of coulom in terminal window
                        used to hadle ouput of printf
parameter: none
return: number of coulom in terminal window
***/
int get_windowSize();
/***
Function Description: this function print all file in dirctory like (ls with 3 flags -R, -l, -a) 
parameter: const char *dir_name: name of dirctory, 
            flag_t flags: struct contain 3 flags (-R, -l, -a)  
return: none
***/
void myls(const char *dir_name, flag_t flags);



#endif


/*** readdir struct definition ***/

/*
 *   struct dirent {
 *       ino_t          d_ino;       /* Inode number *\/
 *       off_t          d_off;       /* Not an offset; see below *\/
 *       unsigned short d_reclen;    /* Length of this record *\/
 *       unsigned char  d_type;      /* Type of file; not supported
 *                                       by all filesystem types *\/
 *       char           d_name[256]; /* Null-terminated filename *\/
 *   };
 */

/*** type definition ***/
/*
 *   DT_BLK      This is a block device.
 *   DT_CHR      This is a character device.
 *   DT_DIR      This is a directory.
 *   DT_FIFO     This is a named pipe (FIFO).
 *   DT_LNK      This is a symbolic link.
 *   DT_REG      This is a regular file.
 *   DT_SOCK     This is a UNIX domain socket.
 *   DT_UNKNOWN  The file type could not be determined.
*/

/*
 *   struct stat {
 *           dev_t     st_dev;         /* ID of device containing file *\/
 *           ino_t     st_ino;         /* Inode number *\/
 *           mode_t    st_mode;        /* File type and mode *\/
 *           nlink_t   st_nlink;       /* Number of hard links *\/
 *           uid_t     st_uid;         /* User ID of owner *\/
 *           gid_t     st_gid;         /* Group ID of owner *\/
 *           dev_t     st_rdev;        /* Device ID (if special file) *\/
 *           off_t     st_size;        /* Total size, in bytes *\/
 *           blksize_t st_blksize;     /* Block size for filesystem I/O *\/
 *           blkcnt_t  st_blocks;      /* Number of 512B blocks allocated *\/
 *
 *           /* Since Linux 2.6, the kernel supports nanosecond
 *               precision for the following timestamp fields.
 *               For the details before Linux 2.6, see NOTES. *\/
 *
 *           struct timespec st_atim;  /* Time of last access *\/
 *           struct timespec st_mtim;  /* Time of last modification *\/
 *           struct timespec st_ctim;  /* Time of last status change *\/
 *
 *       #define st_atime st_atim.tv_sec      /* Backward compatibility /
 *       #define st_mtime st_mtim.tv_sec
 *       #define st_ctime st_ctim.tv_sec
 *   };
 */
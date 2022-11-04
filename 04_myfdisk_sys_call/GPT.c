#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#include "GPT.h"
#include "myfdisk.h"

/***
Function Description: this function parsing partion type and retun string of type name
parameter: const uint64_t type[2]: partion type ID
return: string contain type name
***/
char *GPT_parsingPartionType(const uint64_t type[2])
{
    if(type[0] == Linux_filesystem_1 && type[1] == Linux_filesystem_2) 
        return "Linux filesystem"; 
    else if(type[0] == Linux_home_1 && type[1] == Linux_home_2) 
        return "Linux home";
    else if (type[0] == Microsoft_basic_data_1 && type[1] == Microsoft_basic_data_2)
        return "Microsoft basic data";
    return "";
}
/***
Function Description: this function print header of partion table of GPT format
parameter: none
return: none
***/
void GPT_printPartionTableHeader()
{
    printf(RED "Disklabel type: GPT\n" RESET);
    /* Device        Start      End  Sectors  Size Type */
    printf(CYN"%s %12s %8s %8s %5s %s\n"RESET,
            "Device","Start","End","Sectors","Size","Type");
}
/***
Function Description: this function print colum of partion table of GPT format
parameter: const gpt_PartionEntry *P_Entry: pointer of struct with GPT entry format
return: none
***/
void GPT_printPartionTableColum(const gpt_PartionEntry *P_Entry)
{
    /* Device        Start      End  Sectors  Size Type */
    /* /dev/sdc1      2048  5244927  5242880  2.5G Linux filesystem*/
    uint64_t numOfSectors = P_Entry->lastLBA - P_Entry->firstLBA;
    double partion_size = (double) numOfSectors * SECTOR_SIZE / (1024 * 1024 );

    printf(" %8lu %8lu %8lu %4.*f%c %s\n",
            P_Entry->firstLBA,
            P_Entry->lastLBA,
            numOfSectors,
            (((uint64_t)partion_size % 1024) == 0 || (partion_size < 1024 )) ? 0:1,
            partion_size < 1024 ? partion_size : partion_size/1024,
            partion_size < 1024 ? 'M' : 'G',
            GPT_parsingPartionType(P_Entry->partionType_ID)
            );
}   
/***
Function Description: this function read GPT header format
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
return: none
***/
void GPT_getPartionTableHeader(const char* partion, int fd)
{
    char buff[SECTOR_SIZE];
    /* check correct read */
    if(read(fd,buff,SECTOR_SIZE) == -1)
    {
		printf("Error! read partion file LBA 1\n");
        return;
    }
    gpt_PartionTableHeader *table_header_ptr = (gpt_PartionTableHeader*)&buff[0];
    uint64_t totalSectors = table_header_ptr->lastUsableLBA + 34;
    double partion_size = (double) totalSectors * SECTOR_SIZE / (1024 * 1024 );

    printf("Disk %s %.*f %s, %ld bytes, %ld sectors\n"RESET,
            partion,
            (((uint64_t)partion_size % 1024) == 0 || (partion_size < 1024 )) ? 0:2,
            partion_size < 1024 ? partion_size : partion_size/1024,
            partion_size < 1024 ? "MiB" : "GiB",
            totalSectors*SECTOR_SIZE, 
            totalSectors
            );
}
/***
Function Description: this function get GPT enteries of partions
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
return: none
***/
void GPT_getPartionTableEnteris(const char* partion, int fd)
{
    char buff[SECTOR_SIZE];
    for(int LBA =0; LBA<32 ; LBA++)
    {
        /* check correct read */
        if(read(fd,buff,SECTOR_SIZE) == -1)
        {
            printf("Error! read partion file LBA %d\n",LBA);
            return;
        }
        gpt_entry *table_entry_ptr = (gpt_entry*)&buff[0];
        /* flag for last entry */
        int flag = 0;
        for(int i=0; i<4; i++)
        {
            /* check if last entry */
            if(table_entry_ptr-> entry_ptr[i].firstLBA == 0)
            {
                flag = 1;
                break;
            }
            printf("%s%d ",partion,(LBA*4)+i+1);
            GPT_printPartionTableColum(&(table_entry_ptr->entry_ptr[i]));
        }
        /* check if last entry */
        if(flag)
            break;
    }
}
/***
Function Description: this function print GPT partions
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
return:  none
***/
void GPT_partionTable(const char *partion, int fd)
{
    /*** Read GPT partion table Header ***/
    GPT_getPartionTableHeader(partion, fd);
    /* print partion table header*/
    GPT_printPartionTableHeader();
    /*** Read GPT partion table Enteries ***/
    GPT_getPartionTableEnteris(partion, fd);
}

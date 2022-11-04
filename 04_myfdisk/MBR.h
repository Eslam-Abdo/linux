#include <inttypes.h>

#ifndef _MBR_H_
#define _MBR_H_

/* define struct for MBR partion entry format*/
typedef struct
{
    uint8_t status;
    uint8_t first_chs[3];
    uint8_t partion_type;
    uint8_t last_chs[3];
    uint32_t lba;
    uint32_t sector_count;
}dos_PartionEntry;

/**************************** define of funcs *****************************/
/***
Function Description: this function parsing partion type and retun string of type name
parameter: const uint8_t type: partion type value
return: string contain type name
***/
char *MBR_parsingPartionType(const uint8_t type);
/***
Function Description: this function print header of partion table of MBR or EBR format
parameter: none
return: none
***/
void MBR_printPartionTableHeader();
/***
Function Description: this function print colum of partion table of MBR or EBR format
parameter: const dos_PartionEntry *P_Entry: pointer of struct with MBR entry format
return: none
***/
void MBR_printPartionTableColum(const dos_PartionEntry *P_Entry);
/***
Function Description: this function print EBR partions
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
            uint32_t  ebr_offset : offset of first EBR table
return:  none
***/
void EBR_partionTable(const char *partion, int fd, uint32_t ebr_offset);
/***
Function Description: this function print MBR partions
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
            const dos_PartionEntry *table_entry_ptr : pointer of struct with MBR entry format
return: none
***/
void MBR_partionTable(const char *partion, int fd, const dos_PartionEntry* table_entry_ptr);

#endif
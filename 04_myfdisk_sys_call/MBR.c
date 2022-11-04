#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

#include "myfdisk.h"
#include "MBR.h"

/***
Function Description: this function parsing partion type and retun string of type name
parameter: const uint8_t type: partion type value
return: string contain type name
***/
char *MBR_parsingPartionType(const uint8_t type)
{
    switch (type)
    {
    case 0x00:
        return "Empty"; break;
    case 0x05:
        return "Extended"; break;
    case 0x06:
       return "FAT16"; break;
    case 0x07:
       return "HPFS/NTFS/exFAT"; break;
    case 0x0b: 
        return "FAT32(CHS)"; break;
    case 0x0f:
        return "W95 Ext'd (LBA)"; break; 
    case 0x27:
        return "Hidden NTFS WinRE"; break;
    case 0x82:
        return "Linux swap / Solaris"; break;
    case 0x83:
        return "Linux"; break;
    case 0xee:
        return "GPT protective MBR"; break;
    default:
        return ""; break;
    }
}
/***
Function Description: this function print header of partion table of MBR or EBR format
parameter: none
return: none
***/
void MBR_printPartionTableHeader()
{
    printf(RED "Disklabel type: DOS (MBR)\n" RESET);
    /* Device     Boot     Start       End   Sectors   Size Id Type */
    printf(CYN"%s %8s %9s %9s %9s %6s %s %s\n"RESET,
            "Device","Boot","Start","End","Sectors","Size","Id","Type");
}

/***
Function Description: this function print colum of partion table of MBR or EBR format
parameter: const dos_PartionEntry *P_Entry: pointer of struct with MBR entry format
return: none
***/
void MBR_printPartionTableColum(const dos_PartionEntry *P_Entry)
{
    /* Device     Boot     Start       End   Sectors   Size Id Type */
    /* /dev/sda1  *         2048    104447    102400    50M  7 HPFS/NTFS/exFAT*/
    double partion_size = (double)P_Entry->sector_count * SECTOR_SIZE / (1024 * 1024 );
    printf(" %c %12u %9u %9u %5.*f%c %2x %3s\n",
            P_Entry->status == 0x80 ? '*':' ',
            P_Entry->lba,
            P_Entry->lba + P_Entry->sector_count -1,
            P_Entry->sector_count,
            (((uint32_t)partion_size % 1024) == 0 || (partion_size < 1024 )) ? 0:1,
            partion_size < 1024 ? partion_size : partion_size/1024,
            partion_size < 1024 ? 'M' : 'G',
            P_Entry->partion_type,
            MBR_parsingPartionType(P_Entry->partion_type)
            );
}
/***
Function Description: this function print EBR partions
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
            uint32_t  ebr_offset : offset of first EBR table
return:  none
***/
void EBR_partionTable(const char *partion, int fd, uint32_t ebr_offset)
{
    char buff[SECTOR_SIZE];
    uint64_t ebr_next_offset = 0;
    int i = 5;
    /* read EBR patrion table */
    while(ebr_offset != -1)
    {
        /* convert num of sectors to bytes */
        ebr_next_offset = (ebr_offset + ebr_next_offset) * SECTOR_SIZE;
        /* reposition file offset at start of next ebr */
        if(lseek(fd,ebr_next_offset,SEEK_SET) == -1)
        {
            printf("Error! reposition file offset\n");
            return;
        }
        /* read sector of EBR */
        if(read(fd,buff,SECTOR_SIZE) == -1)
        {
		    printf("Error! read partion file\n");
            return;
        }
        dos_PartionEntry *table_entry_ptr = (dos_PartionEntry*)&buff[446];
        /* print partion details */
        printf("%s%d ",partion,i++);
        table_entry_ptr[0].lba += ebr_next_offset/SECTOR_SIZE ;
        MBR_printPartionTableColum(&table_entry_ptr[0]);
        /* update next EBR offset */
        ebr_next_offset = table_entry_ptr[1].lba;
        /* check if end of extented partion */
        if(table_entry_ptr[1].lba == 0)
            break;
    }
}
/***
Function Description: this function print MBR partions
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
            const dos_PartionEntry *table_entry_ptr : pointer of struct with MBR entry format
return: none
***/
void MBR_partionTable(const char *partion, int fd, const dos_PartionEntry* table_entry_ptr)
{
    /* external partion offset */
    uint32_t ebr_offset = -1;
    /* print partion table header*/
    MBR_printPartionTableHeader();
    /* read MBR patrion table */
    for(int i=0; i<4; i++)
    {
        /* skip empty partion*/
        if(table_entry_ptr[i].partion_type == 0)
            continue;
        printf("%s%d ",partion,i+1);
        MBR_printPartionTableColum(&table_entry_ptr[i]);
        /* check for extetend partion */
        if(table_entry_ptr[i].partion_type == 0x05)
            ebr_offset = table_entry_ptr[i].lba;
    }
    if(ebr_offset != -1)
        EBR_partionTable(partion,fd,ebr_offset);
}

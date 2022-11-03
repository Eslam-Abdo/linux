#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#define SECTOR_SIZE         512

typedef struct
{
    uint8_t status;
    uint8_t first_chs[3];
    uint8_t partion_type;
    uint8_t last_chs[3];
    uint32_t lba;
    uint32_t sector_count;
}dos_PartionEntry;


void print_partionTable_head()
{
    /* Device     Boot     Start       End   Sectors   Size Id Type */
    printf("%s %8s %9s %9s %9s %6s %s %s\n",
            "Device","Boot","Start","End","Sectors","Size","Id","Type");
}
char *parsing_partionType(const uint8_t type)
{
    switch (type)
    {
    case 0x00:
        return "Empty"; break;
    case 0x05:
        return "Extended"; break;
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
void print_partionTable_col(const dos_PartionEntry *P_Entry)
{
    /* Device     Boot     Start       End   Sectors   Size Id Type */
    /* /dev/sda1  *         2048    104447    102400    50M  7 HPFS/NTFS/exFAT*/
    double partion_size = (double)P_Entry->sector_count * 512 / (1024 * 1024 );
    printf(" %c %12u %9u %9u %6.*f%c %x %s\n",
            P_Entry->status == 0x80 ? '*':' ',
            P_Entry->lba,
            P_Entry->lba + P_Entry->sector_count -1,
            P_Entry->sector_count,
            (((uint32_t)partion_size % 1024) == 0 || (partion_size < 1024 )) ? 0:1,
            partion_size < 1024 ? partion_size : partion_size/1024,
            partion_size < 1024 ? 'M' : 'G',
            P_Entry->partion_type,
            parsing_partionType(P_Entry->partion_type)
            );
}
uint32_t MBR_partionTable(const char *partion, const dos_PartionEntry* table_entry_ptr)
{
    /* external partion offset */
    uint32_t ebr_offset = -1;
    /* print table header */
    print_partionTable_head();
    /* read MBR patrion table */
    for(int i=0; i<4; i++)
    {
        /* skip empty partion*/
        if(table_entry_ptr[i].partion_type == 0)
            continue;
        printf("%s%d ",partion,i+1);
        print_partionTable_col(&table_entry_ptr[i]);
        /* check for extetend partion */
        if(table_entry_ptr[i].partion_type == 0x05)
            ebr_offset = table_entry_ptr[i].lba;
    }
    return ebr_offset;
}
void EBR_partionTable(const char *partion, int fd, uint32_t ebr_offset)
{
    char buff[SECTOR_SIZE];
    uint32_t ebr_next_offset = 0;
    int i = 5;
    /* read EBR patrion table */
    while(ebr_offset != -1)
    {
        /* convert num of sectors to bytes */
        ebr_next_offset = (ebr_offset + ebr_next_offset) * 512;
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
        table_entry_ptr[0].lba += ebr_next_offset/512 ;
        print_partionTable_col(&table_entry_ptr[0]);
        /* update next EBR offset */
        ebr_next_offset = table_entry_ptr[1].lba;
        /* check if end of extented partion */
        if(table_entry_ptr[1].lba == 0)
            break;
    }
}
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
		printf("Error! read partion file\n");
        return;
    }
    uint8_t type = buff[450];
    /* check for table type GPT or MBR */
    printf("Disklabel type: ");
    if(type == 0xee)
    {
        printf("GPT\n");
    }
    else
    {
        printf("DOS\n");
        /* print MBR table */
        uint32_t offset = MBR_partionTable(partion, (dos_PartionEntry*)&buff[446]);
        /* check if external partion founded */
        if(offset != -1)
            EBR_partionTable(partion,fd,offset);
    }
    
    /* check correct close */
	if(close(fd) == -1)
		printf("Error! close partion file\n");
}

int main(int argc, char *argv[])
{
    if(argc != 2)
		printf("Error! please enter partion argument\n");
	else
        myfdisk(argv[1]);

    printf("\n");
    return 0;
}
#ifndef _GPT_H_
#define _GPT_H_

/**************************** define of GPT structs *****************************/
/* define struct for GPT partion table header format*/
typedef struct
{
    char signature[8];
    uint32_t revision;
    uint32_t headerSize;
    uint32_t headerCRC32;
    uint32_t reserved;
    uint64_t primaryLBA;
    uint64_t backupLBA;
    uint64_t firstUsableLBA;
    uint64_t lastUsableLBA;
    uint64_t uniqueGUID1;
    uint64_t uniqueGUID2;
    uint64_t firstEntryLBA;
    uint32_t numberOfEntries;
    uint32_t sizeOfEntry;
    uint32_t EntriesCRC32;
}gpt_PartionTableHeader;

/* define struct for GPT partion entry format*/
typedef struct
{
    uint64_t partionType_ID[2];
    uint64_t partion_ID[2];
    uint64_t firstLBA;
    uint64_t lastLBA;
    uint64_t attributeFlags;
    char     patrion_name[72];
}gpt_PartionEntry;

/* define struct for GPT partion entry format with 4 entry = 1 LBA = 1 sector */
typedef struct 
{
    gpt_PartionEntry entry_ptr[4];

}gpt_entry;


/**************************** define of GPT pation typr ID *********************/
#define Linux_filesystem_1            (uint64_t)0x477284830fc63daf
#define Linux_filesystem_2            (uint64_t)0xe47d47d8693d798e
#define Linux_home_1                  (uint64_t)0x4f132eb4933ac7e1
#define Linux_home_2                  (uint64_t)0x15f9aee2140e44b8
#define Microsoft_basic_data_1        (uint64_t)0x4433b9e5ebd0a0a2
#define Microsoft_basic_data_2        (uint64_t)0xc79926b7b668c087

/**************************** define of funcs *****************************/
/***
Function Description: this function parsing partion type and retun string of type name
parameter: const uint64_t type[2]: partion type ID
return: string contain type name
***/
char *GPT_parsingPartionType(const uint64_t type[2]);
/***
Function Description: this function print header of partion table of GPT format
parameter: none
return: none
***/
void GPT_printPartionTableHeader();
/***
Function Description: this function print colum of partion table of GPT format
parameter: const gpt_PartionEntry *P_Entry: pointer of struct with GPT entry format
return: none
***/
void GPT_printPartionTableColum(const gpt_PartionEntry *P_Entry);
/***
Function Description: this function read GPT header format
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
return: none
***/
void GPT_getPartionTableHeader(const char* partion, int fd);
/***
Function Description: this function get GPT enteries of partions
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
return: none
***/
void GPT_getPartionTableEnteris(const char* partion, int fd);
/***
Function Description: this function print GPT partions
parameter:  const char *partion : partoin name this should by user input in argv[1] e.g. "/dev/sda",
            int fd : file descriptor,
return:  none
***/
void GPT_partionTable(const char *partion, int fd);



#endif
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "myls.h"

int main(int argc, char *argv[])
{
    flag_t flags = {0,0,0};
    int opt;
    
    /* parsing argv option -R -l -a */
    /*
     * put ':' in the starting of the
     * string so that program can
     * distinguish between '?' and ':'
     */
    while((opt = getopt(argc, argv, "Rla")) != -1)
    {
        switch(opt)
        {
            case 'R':
                flags.recursive = 1;    break;
            case 'l':
                flags.list_files = 1;    break;
            case 'a':
                flags.all_files = 1;    break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }
    /* if no dirctory input used current location */
    if(optind == argc)
        myls("./",flags);

    // optind is for the extra arguments
    // which are not parsed
    for(; optind < argc; optind++)
        myls(argv[optind],flags);

    return 0;
}

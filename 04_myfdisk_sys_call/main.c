#include <stdio.h>

#include "MBR.h"
#include "GPT.h"
#include "myfdisk.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
		printf("Error! please enter partion argument\n");
	else
        myfdisk(argv[1]);

    printf("\n");
    return 0;
}
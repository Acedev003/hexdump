#include<stdio.h>
#include "hexdump.h"

#define HEXDUMP_VERSION "0.0.1"

int main(int argc,char* argv[])
{
    printf("Hexdump:v%s \n\n",HEXDUMP_VERSION);
    
    print_hex(argv[argc-1]);

    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<getopt.h>
#include<stdbool.h>
#include "hexdump.h"

void print_usage(char *program_name);

int main(int argc,char* argv[])
{

    uint64_t start_byte  = 0;
    uint64_t no_of_bytes = 0;

    int  file_name_index = 0;
    bool save_to_file    = false;
    
    char *number_end_ptr;

    int option;
    while((option = getopt(argc,argv,"hs:n:f:")) != -1)
    {
        switch(option)
        {
            case 'h' : print_usage(argv[0]);
                       return EXIT_SUCCESS;

            case 's' : start_byte = strtoull(optarg,&number_end_ptr,10);
                       if(number_end_ptr == optarg || *number_end_ptr != '\0')
                       { 
                           fprintf(stderr,"\nError: Failed to parse -s value");
                           return EXIT_FAILURE;
                       }
                       if((start_byte == UINT64_MAX) && (errno == ERANGE))
                       {
                           fprintf(stderr,"\nError: Input value (-s) out of range");
                           return EXIT_FAILURE;
                       }
                       break;

            case 'n' : no_of_bytes = strtoull(optarg,&number_end_ptr,10);
                       if (number_end_ptr == optarg || *number_end_ptr != '\0')
                       {
                           fprintf(stderr, "\nError: Failed to parse -n value");
                           return EXIT_FAILURE;
                       }
                       if ((start_byte == UINT64_MAX) && (errno == ERANGE))
                       {
                           fprintf(stderr, "\nError: Input value (-n) out of range");
                           return EXIT_FAILURE;
                       }
                       break;

            case 'f' : save_to_file    = true;
                       file_name_index = optind - 1;
                       break;

            default  : print_usage(argv[0]);
                       return EXIT_FAILURE;
        }
    }

    if(optind + 1 != argc)
    {
        printf("Error: Arguments in wrong format\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if(!save_to_file)
    {
        print_hex(argv[argc-1],start_byte,no_of_bytes,NULL);
    }
    else
    {
        print_hex(argv[argc-1],start_byte,no_of_bytes,argv[file_name_index]);
    }

    return EXIT_SUCCESS;
}


void print_usage(char *program_name)
{
    printf("\nUsage: %s [options] filename\n\n",program_name);
    printf("Options:\n");
    printf(" -s: skips specified amount of bytes (Default value: 0)\n");
    printf(" -n: interpret only 'n' bytes of input (Default value: 0) [Prints entire file if value is 0 or below]\n");
    printf(" -f: Saves output into the specified file\n");
    printf(" -h: View help\n");
}
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<stdint.h>
#include<errno.h>
#include "hexdump.h"

int main(int argc,char* argv[])
{

    uint64_t start_byte  = 0;
    uint64_t no_of_bytes = 0;

    int save_to_file = FALSE;
    int file_name_index = 0;

    char *number_end_ptr;


    printf("Hexdump:v%s \n\n",HEXDUMP_VERSION);

    if(argc % 2 != 0)
    {
        fprintf(stderr,"\nIncomplete Parameters. Type [%s -h] for all available options.\n",argv[0]);
        return -1;
    }


    for(int i = 1; i < argc; i+=2)
    {
        if((argv[i][0] == '-') && (strlen(argv[i]) == 2))
        {
            switch(argv[i][1])
            {
                case 'h':
                case '?': printf("A simple command line utility to view the hex contents of any given file.\n");
                          printf("\nUsage:\n\n");
                          printf("   1.%s -s <start_byte> -n <no_of_bytes> -f <output_file> <input_file_name>\n",argv[0]);
                          printf("   2.%s -h (or -?) for help\n\n",argv[0]);
                          printf(" -s: starts printing values after the specified amount of bytes (Default value: 0)\n");
                          printf(" -n: prints values for 'n' bytes (Default value: 0)\n");
                          printf(" -f: Saves output into the specified file instead of printing to console\n\n");
                          printf(" -h or -?: View help\n");
                          return 0;

                case 's': start_byte = strtoull(argv[i+1],&number_end_ptr,10);
                          if(number_end_ptr == argv[i+1] || *number_end_ptr != '\0')
                          {
                              fprintf(stderr,"\nError: Failed to parse -s value");
                              return -1;
                          }
                          if((start_byte == UINT64_MAX) && (errno == ERANGE))
                          {
                              fprintf(stderr,"\nError: Input value (-s) out of range");
                              return -1;
                          }
                          break;

                case 'n': no_of_bytes = strtoull(argv[i+1],&number_end_ptr,10);
                          if(number_end_ptr == argv[i+1] || *number_end_ptr != '\0')
                          {
                              fprintf(stderr,"\nError: Failed to parse -n value");
                              return -1;
                          }
                          if((start_byte == UINT64_MAX) && (errno == ERANGE))
                          {
                              fprintf(stderr,"\nError: Input value (-n) out of range");
                              return -1;
                          }
                          break;
                          
                case 'f': save_to_file    = TRUE;
                          file_name_index = i + 1;
                          break;
            }
        }
    }

    if(!save_to_file)
    {
        print_hex(argv[argc-1],start_byte,no_of_bytes);
    }
    else
    {
        //TODO
    }

    return 0;
}
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
                case 'h': printf("Usage:\n\n");
                          printf("   %s [options] <file_name>\n\n",argv[0]);
                          printf("Options:\n\n");
                          printf(" -s: skips specified amount of bytes (Default value: 0)\n");
                          printf(" -n: interpret only 'n' bytes of input (Default value: 0) [Prints entire file if value is 0 or below]\n");
                          printf(" -f: Saves output into the specified file\n");
                          printf(" -h: View help\n");
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
        print_hex(argv[argc-1],start_byte,no_of_bytes,NULL);
    }
    else
    {
        print_hex(argv[argc-1],start_byte,no_of_bytes,argv[file_name_index]);
    }

    return 0;
}
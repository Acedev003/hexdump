#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<getopt.h>
#include<stdbool.h>
#include<sys/stat.h>

#define HEXDUMP_VERSION "0.1.0"

#define LINE_COUNTER_WIDTH   8
#define BYTES_PER_LINE       16

#define PRINTABLE_MIN_ASCII  32
#define PRINTABLE_MAX_ASCII  126    // https://en.wikipedia.org/wiki/ASCII#Printable_characters

#define PRINTF_BUFFER_SIZE   3200

void print_usage(char *program_name);
int  print_hex(char *input_file_name,uint64_t start_byte,uint64_t no_of_bytes,char *output_file_name);

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

            case 's' : start_byte = strtoull(optarg,&number_end_ptr,0);
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

            case 'n' : no_of_bytes = strtoull(optarg,&number_end_ptr,0);
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


int print_hex(char *input_file_name,uint64_t start_byte,uint64_t no_of_bytes,char *output_file_name)
{
    FILE *input_stream;
    FILE *output_stream;

    struct stat file_data;  

    input_stream = fopen(input_file_name,"rb");
    if(input_stream == NULL)
    {
        fprintf(stderr,"Error: Failed to open file [%s]\n",input_file_name);
        return -1;
    }

    if(output_file_name == NULL)
    {
        output_stream = stdout;
    }
    else
    {
        output_stream = fopen(output_file_name,"wb");
        if(output_stream == NULL)
        {
            fprintf(stderr,"Error: Failed to open file [%s]\n",output_file_name);
            return -1;
        }
    }

    

    fprintf(output_stream,"Hexdump:v%s \n\n",HEXDUMP_VERSION);


    fprintf(output_stream,"File : %s\n",input_file_name);
    if(stat(input_file_name,&file_data) == 0)
    {
        fprintf(output_stream,"Size : %ld bytes\n\n",file_data.st_size);
    }
    else
    {
        fprintf(stderr,"Error: Failed to parse file size");
        return -1;
    }

    
    if(file_data.st_size >= UINT64_MAX-1000)
    {
        fprintf(stderr,"\nWARN: Too large file. Exiting....\n");
        return 0;
    }

    if((fseek(input_stream,start_byte,SEEK_SET) != 0) || (start_byte > file_data.st_size ))
    {
        fprintf(stderr,"Error: Failed to set start position at byte %lld\n",start_byte);
        return -1;
    }

    if(no_of_bytes <= 0)
    {
        no_of_bytes = file_data.st_size;
    }



    int int_byte;               // Refer NOTES (LL1)
    
    int line_cursor   = 0;

    uint64_t byte_cursor = ftell(input_stream); 
    uint64_t line_count = byte_cursor / BYTES_PER_LINE;

    char printf_buffer[PRINTF_BUFFER_SIZE];
    unsigned char line[BYTES_PER_LINE];                     // Refer NOTES (LL2)

    setvbuf(output_stream,printf_buffer,_IOFBF,sizeof(printf_buffer));

    if(byte_cursor % BYTES_PER_LINE != 0)
    {
        int stop_execution = false;

        for(uint64_t i = 0; i < BYTES_PER_LINE; i++)
        {
            line[i] = 0;   
        }

        for(uint64_t i = byte_cursor % BYTES_PER_LINE; i < BYTES_PER_LINE; i++)
        {
            line[i] = fgetc(input_stream);
            no_of_bytes--;

            if(line[i] == EOF || no_of_bytes == 0)
            {
                stop_execution = true;
                break;
            }       
        }

        fprintf(output_stream,
                "%.*llx  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
                LINE_COUNTER_WIDTH, line_count,
                line[0],line[1],line[2],line[3],line[4],line[5],line[6],line[7],
                line[8],line[9],line[10],line[11],line[12],line[13],line[14],line[15],
                (line[ 0] >= PRINTABLE_MIN_ASCII && line[ 0] <= PRINTABLE_MAX_ASCII) ? line[ 0] : '.',
                (line[ 1] >= PRINTABLE_MIN_ASCII && line[ 1] <= PRINTABLE_MAX_ASCII) ? line[ 1] : '.',
                (line[ 2] >= PRINTABLE_MIN_ASCII && line[ 2] <= PRINTABLE_MAX_ASCII) ? line[ 2] : '.',
                (line[ 3] >= PRINTABLE_MIN_ASCII && line[ 3] <= PRINTABLE_MAX_ASCII) ? line[ 3] : '.',
                (line[ 4] >= PRINTABLE_MIN_ASCII && line[ 4] <= PRINTABLE_MAX_ASCII) ? line[ 4] : '.',
                (line[ 5] >= PRINTABLE_MIN_ASCII && line[ 5] <= PRINTABLE_MAX_ASCII) ? line[ 5] : '.',
                (line[ 6] >= PRINTABLE_MIN_ASCII && line[ 6] <= PRINTABLE_MAX_ASCII) ? line[ 6] : '.',
                (line[ 7] >= PRINTABLE_MIN_ASCII && line[ 7] <= PRINTABLE_MAX_ASCII) ? line[ 7] : '.',
                (line[ 8] >= PRINTABLE_MIN_ASCII && line[ 8] <= PRINTABLE_MAX_ASCII) ? line[ 8] : '.',
                (line[ 9] >= PRINTABLE_MIN_ASCII && line[ 9] <= PRINTABLE_MAX_ASCII) ? line[ 9] : '.',
                (line[10] >= PRINTABLE_MIN_ASCII && line[10] <= PRINTABLE_MAX_ASCII) ? line[10] : '.',
                (line[11] >= PRINTABLE_MIN_ASCII && line[11] <= PRINTABLE_MAX_ASCII) ? line[11] : '.',
                (line[12] >= PRINTABLE_MIN_ASCII && line[12] <= PRINTABLE_MAX_ASCII) ? line[12] : '.',
                (line[13] >= PRINTABLE_MIN_ASCII && line[13] <= PRINTABLE_MAX_ASCII) ? line[13] : '.',
                (line[14] >= PRINTABLE_MIN_ASCII && line[14] <= PRINTABLE_MAX_ASCII) ? line[14] : '.',
                (line[15] >= PRINTABLE_MIN_ASCII && line[15] <= PRINTABLE_MAX_ASCII) ? line[15] : '.'
                );   
                line_count++;

                if(stop_execution)
                {
                    return 0;
                }
    }

    while( (int_byte = fgetc(input_stream)) != EOF && no_of_bytes != 0)
    {   
        if( line_cursor == BYTES_PER_LINE )
        {
            fprintf(output_stream,
                "%.*llx  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
                LINE_COUNTER_WIDTH, line_count,
                line[0],line[1],line[2],line[3],line[4],line[5],line[6],line[7],
                line[8],line[9],line[10],line[11],line[12],line[13],line[14],line[15],
                (line[ 0] >= PRINTABLE_MIN_ASCII && line[ 0] <= PRINTABLE_MAX_ASCII) ? line[ 0] : '.',
                (line[ 1] >= PRINTABLE_MIN_ASCII && line[ 1] <= PRINTABLE_MAX_ASCII) ? line[ 1] : '.',
                (line[ 2] >= PRINTABLE_MIN_ASCII && line[ 2] <= PRINTABLE_MAX_ASCII) ? line[ 2] : '.',
                (line[ 3] >= PRINTABLE_MIN_ASCII && line[ 3] <= PRINTABLE_MAX_ASCII) ? line[ 3] : '.',
                (line[ 4] >= PRINTABLE_MIN_ASCII && line[ 4] <= PRINTABLE_MAX_ASCII) ? line[ 4] : '.',
                (line[ 5] >= PRINTABLE_MIN_ASCII && line[ 5] <= PRINTABLE_MAX_ASCII) ? line[ 5] : '.',
                (line[ 6] >= PRINTABLE_MIN_ASCII && line[ 6] <= PRINTABLE_MAX_ASCII) ? line[ 6] : '.',
                (line[ 7] >= PRINTABLE_MIN_ASCII && line[ 7] <= PRINTABLE_MAX_ASCII) ? line[ 7] : '.',
                (line[ 8] >= PRINTABLE_MIN_ASCII && line[ 8] <= PRINTABLE_MAX_ASCII) ? line[ 8] : '.',
                (line[ 9] >= PRINTABLE_MIN_ASCII && line[ 9] <= PRINTABLE_MAX_ASCII) ? line[ 9] : '.',
                (line[10] >= PRINTABLE_MIN_ASCII && line[10] <= PRINTABLE_MAX_ASCII) ? line[10] : '.',
                (line[11] >= PRINTABLE_MIN_ASCII && line[11] <= PRINTABLE_MAX_ASCII) ? line[11] : '.',
                (line[12] >= PRINTABLE_MIN_ASCII && line[12] <= PRINTABLE_MAX_ASCII) ? line[12] : '.',
                (line[13] >= PRINTABLE_MIN_ASCII && line[13] <= PRINTABLE_MAX_ASCII) ? line[13] : '.',
                (line[14] >= PRINTABLE_MIN_ASCII && line[14] <= PRINTABLE_MAX_ASCII) ? line[14] : '.',
                (line[15] >= PRINTABLE_MIN_ASCII && line[15] <= PRINTABLE_MAX_ASCII) ? line[15] : '.'
                );

                line_count  += 16;
                line_cursor  =  0;

                line[line_cursor] = int_byte;
                no_of_bytes--;
                line_cursor++;
        }
        else
        {
            line[line_cursor] = int_byte;
            no_of_bytes--;
            line_cursor++;
        }

        byte_cursor++;
    }
    fflush(output_stream);

    for (int i = line_cursor; i < BYTES_PER_LINE; i++)
    {
        line[i] = 0;
    }
    fprintf(output_stream,
        "%.*llx  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
        LINE_COUNTER_WIDTH, line_count,
        line[0], line[1], line[2], line[3], line[4], line[5], line[6], line[7],
        line[8], line[9], line[10], line[11], line[12], line[13], line[14], line[15],
        (line[ 0] >= PRINTABLE_MIN_ASCII && line[ 0] <= PRINTABLE_MAX_ASCII) ? line[ 0] : '.',
        (line[ 1] >= PRINTABLE_MIN_ASCII && line[ 1] <= PRINTABLE_MAX_ASCII) ? line[ 1] : '.',
        (line[ 2] >= PRINTABLE_MIN_ASCII && line[ 2] <= PRINTABLE_MAX_ASCII) ? line[ 2] : '.',
        (line[ 3] >= PRINTABLE_MIN_ASCII && line[ 3] <= PRINTABLE_MAX_ASCII) ? line[ 3] : '.',
        (line[ 4] >= PRINTABLE_MIN_ASCII && line[ 4] <= PRINTABLE_MAX_ASCII) ? line[ 4] : '.',
        (line[ 5] >= PRINTABLE_MIN_ASCII && line[ 5] <= PRINTABLE_MAX_ASCII) ? line[ 5] : '.',
        (line[ 6] >= PRINTABLE_MIN_ASCII && line[ 6] <= PRINTABLE_MAX_ASCII) ? line[ 6] : '.',
        (line[ 7] >= PRINTABLE_MIN_ASCII && line[ 7] <= PRINTABLE_MAX_ASCII) ? line[ 7] : '.',
        (line[ 8] >= PRINTABLE_MIN_ASCII && line[ 8] <= PRINTABLE_MAX_ASCII) ? line[ 8] : '.',
        (line[ 9] >= PRINTABLE_MIN_ASCII && line[ 9] <= PRINTABLE_MAX_ASCII) ? line[ 9] : '.',
        (line[10] >= PRINTABLE_MIN_ASCII && line[10] <= PRINTABLE_MAX_ASCII) ? line[10] : '.',
        (line[11] >= PRINTABLE_MIN_ASCII && line[11] <= PRINTABLE_MAX_ASCII) ? line[11] : '.',
        (line[12] >= PRINTABLE_MIN_ASCII && line[12] <= PRINTABLE_MAX_ASCII) ? line[12] : '.',
        (line[13] >= PRINTABLE_MIN_ASCII && line[13] <= PRINTABLE_MAX_ASCII) ? line[13] : '.',
        (line[14] >= PRINTABLE_MIN_ASCII && line[14] <= PRINTABLE_MAX_ASCII) ? line[14] : '.',
        (line[15] >= PRINTABLE_MIN_ASCII && line[15] <= PRINTABLE_MAX_ASCII) ? line[15] : '.');

    fclose(input_stream);
    fclose(output_stream);
    return 0;
}
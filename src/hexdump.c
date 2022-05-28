#include<stdio.h>
#include<stdint.h>
#include<sys/stat.h>
#include"hexdump.h"

#define LINE_COUNTER_WIDTH   8
#define BYTES_PER_LINE       16

#define PRINTABLE_MIN_ASCII  32
#define PRINTABLE_MAX_ASCII  126    // https://en.wikipedia.org/wiki/ASCII#Printable_characters


int print_hex(char *file_name,uint64_t start_byte,uint64_t no_of_bytes)
{
    FILE *input_file;
    struct stat file_data;  

    input_file = fopen(file_name,"rb");
    if(input_file == NULL)
    {
        fprintf(stderr,"Error: Failed to open file [%s]\n",file_name);
        return -1;
    }

    printf("File : %s\n",file_name);
    if(stat(file_name,&file_data) == 0)
    {
        printf("Size : %lld bytes\n\n",file_data.st_size);
    }
    else
    {
        printf("Error: Failed to parse file size");
        return -1;
    }

    
    if(file_data.st_size >= UINT64_MAX-1000)
    {
        fprintf(stderr,"\nWARN: Too large file. Exiting....\n");
        return 0;
    }

    if((fseek(input_file,start_byte,SEEK_SET) != 0) || (start_byte > file_data.st_size ))
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
    int last_line_printed = FALSE;

    uint64_t byte_cursor = ftell(input_file); 
    uint64_t line_count = byte_cursor / BYTES_PER_LINE;

    unsigned char printf_buffer[3200];
    unsigned char line[BYTES_PER_LINE];                     // Refer NOTES (LL2)

    setvbuf(stdout,printf_buffer,_IOFBF,sizeof(printf_buffer));


    if(byte_cursor % BYTES_PER_LINE != 0)
    {
        int stop_execution = FALSE;

        for(uint64_t i = 0; i < BYTES_PER_LINE; i++)
        {
            line[i] = 0;   
        }

        for(uint64_t i = byte_cursor % BYTES_PER_LINE; i < BYTES_PER_LINE; i++)
        {
            line[i] = fgetc(input_file);
            no_of_bytes--;

            if(line[i] == EOF || no_of_bytes == 0)
            {
                stop_execution = TRUE;
                break;
            }       
        }

        printf(
                "%.*x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
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

    while( (int_byte = fgetc(input_file)) != EOF && no_of_bytes != 0)
    {   
        if( line_cursor == BYTES_PER_LINE )
        {
            printf(
                "%.*x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
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
    fflush(stdout);

    for (int i = line_cursor; i < BYTES_PER_LINE; i++)
    {
        line[i] = 0;
    }
    printf(
        "%.*x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
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

    fclose(input_file);
    return 0;
}
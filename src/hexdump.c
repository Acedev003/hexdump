#include<ctype.h>
#include<stdio.h>
#include<stdint.h>
#include<sys/stat.h>
#include"hexdump.h"

#define LINE_COUNTER_WIDTH   8
#define BYTES_PER_LINE       16

#define PRINTF_BUFFER_SIZE   3200

/* Prints out the ASCII-view sidebar in canonical mode
 * e.g. |hello..| for 'h','e','l','l','o','\r','\n'.
 * Does not include a trailing newline. */
void print_sidebar(FILE *output_stream, char *line, int line_len)
{
    int i;

    fputc('|', output_stream);
    for(i = 0; i < line_len; ++i){
        fputc(isprint(line[i]) ? line[i] : '.', output_stream);
    }
    fputc('|', output_stream);
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
                "%.*llx  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  ",
                LINE_COUNTER_WIDTH, line_count,
                line[0],line[1],line[2],line[3],line[4],line[5],line[6],line[7],
                line[8],line[9],line[10],line[11],line[12],line[13],line[14],line[15]
               	);

        print_sidebar(output_stream, line, 16);
        fputc('\n', output_stream);

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
                "%.*llx  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  ",
                LINE_COUNTER_WIDTH, line_count,
                line[0],line[1],line[2],line[3],line[4],line[5],line[6],line[7],
                line[8],line[9],line[10],line[11],line[12],line[13],line[14],line[15]
                );
            print_sidebar(output_stream, line, 16);
            fputc('\n', output_stream);

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
        "%.*llx  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  ",
        LINE_COUNTER_WIDTH, line_count,
        line[0], line[1], line[2], line[3], line[4], line[5], line[6], line[7],
        line[8], line[9], line[10], line[11], line[12], line[13], line[14], line[15]);
    print_sidebar(output_stream, line, 16);
    fputc('\n', output_stream);

    fclose(input_stream);
    fclose(output_stream);
    return 0;
}

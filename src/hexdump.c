#include<stdio.h>
#include<sys/stat.h>

#define MAX_LINE_BITS   8
#define BYTES_PER_LINE  16
#define LINES_PER_BLOCK 100
#define BLOCK_SIZE      (MAX_LINE_BITS + 2 + (3*BYTES_PER_LINE) + 2 + 1 + BYTES_PER_LINE + 1 + 1)*LINES_PER_BLOCK + 1
#define PRINTABLE_MIN   32
#define PRINTABLE_MAX   126

#define FALSE 0
#define TRUE  1

void print_file_metadata(char* file_name)
{
    struct stat file_data;  

    printf("File : %s\n",file_name);
    if(stat(file_name,&file_data) == 0)
    {
        printf("Size : %ld bytes\n",file_data.st_size);
    }
    printf("\n");
}

int print_hex(char *file_name)
{
    FILE *input_file;

    input_file = fopen(file_name,"rb");
    if(input_file == NULL)
    {
        printf("Error: Failed to open file [%s]\n",file_name);
        return -1;
    }

    print_file_metadata(file_name);

    /*
    * Scratched my head here for 1 week ..... 
    * Answer: https://stackoverflow.com/questions/13830338/377-character-in-c#13830379 
    *         http://c-faq.com/stdio/getcharc.html
    * 
    */

    int int_byte; 
    int line_cursor   = 0;
    int line_printed = FALSE;

    unsigned long int bytes_read = 0; 
    unsigned long int line_count = 0;

    unsigned char buffer[3200];

    /****
     * 
     * Another day spent scratching over here 
     * Solution : https://stackoverflow.com/questions/15823597/use-printf-to-print-character-string-in-hexadecimal-format-distorted-results
     * 
     ****/
    unsigned char line[BYTES_PER_LINE];

    setvbuf(stdout,buffer,_IOFBF,sizeof(buffer));

    while( (int_byte = fgetc(input_file)) != EOF)
    {   
        if( line_cursor == BYTES_PER_LINE )
        {
            printf(
                "%.*x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
                MAX_LINE_BITS, line_count,
                line[0],line[1],line[2],line[3],line[4],line[5],line[6],line[7],
                line[8],line[9],line[10],line[11],line[12],line[13],line[14],line[15],
                (line[ 0] >= PRINTABLE_MIN && line[ 0] <= PRINTABLE_MAX) ? line[ 0] : '.',
                (line[ 1] >= PRINTABLE_MIN && line[ 1] <= PRINTABLE_MAX) ? line[ 1] : '.',
                (line[ 2] >= PRINTABLE_MIN && line[ 2] <= PRINTABLE_MAX) ? line[ 2] : '.',
                (line[ 3] >= PRINTABLE_MIN && line[ 3] <= PRINTABLE_MAX) ? line[ 3] : '.',
                (line[ 4] >= PRINTABLE_MIN && line[ 4] <= PRINTABLE_MAX) ? line[ 4] : '.',
                (line[ 5] >= PRINTABLE_MIN && line[ 5] <= PRINTABLE_MAX) ? line[ 5] : '.',
                (line[ 6] >= PRINTABLE_MIN && line[ 6] <= PRINTABLE_MAX) ? line[ 6] : '.',
                (line[ 7] >= PRINTABLE_MIN && line[ 7] <= PRINTABLE_MAX) ? line[ 7] : '.',
                (line[ 8] >= PRINTABLE_MIN && line[ 8] <= PRINTABLE_MAX) ? line[ 8] : '.',
                (line[ 9] >= PRINTABLE_MIN && line[ 9] <= PRINTABLE_MAX) ? line[ 9] : '.',
                (line[10] >= PRINTABLE_MIN && line[10] <= PRINTABLE_MAX) ? line[10] : '.',
                (line[11] >= PRINTABLE_MIN && line[11] <= PRINTABLE_MAX) ? line[11] : '.',
                (line[12] >= PRINTABLE_MIN && line[12] <= PRINTABLE_MAX) ? line[12] : '.',
                (line[13] >= PRINTABLE_MIN && line[13] <= PRINTABLE_MAX) ? line[13] : '.',
                (line[14] >= PRINTABLE_MIN && line[14] <= PRINTABLE_MAX) ? line[14] : '.',
                (line[15] >= PRINTABLE_MIN && line[15] <= PRINTABLE_MAX) ? line[15] : '.'
                );

                line_count  += 16;
                line_cursor  =  0;
                line_printed = TRUE;

                line[line_cursor] = int_byte;
                line_cursor++;
        }
        else
        {
            line_printed = FALSE;
            line[line_cursor] = int_byte;
            line_cursor++;
        }

        bytes_read++;
    }
    fflush(stdout); 

    if(!line_printed)
    {   
        int i;
        for( i = line_cursor; i < BYTES_PER_LINE; i++)
        {
            line[i] = 0;       
        }
        printf(
                "%.*x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
                MAX_LINE_BITS, line_count,
                line[0],line[1],line[2],line[3],line[4],line[5],line[6],line[7],
                line[8],line[9],line[10],line[11],line[12],line[13],line[14],line[15],
                (line[ 0] >= PRINTABLE_MIN && line[ 0] <= PRINTABLE_MAX) ? line[ 0] : '.',
                (line[ 1] >= PRINTABLE_MIN && line[ 1] <= PRINTABLE_MAX) ? line[ 1] : '.',
                (line[ 2] >= PRINTABLE_MIN && line[ 2] <= PRINTABLE_MAX) ? line[ 2] : '.',
                (line[ 3] >= PRINTABLE_MIN && line[ 3] <= PRINTABLE_MAX) ? line[ 3] : '.',
                (line[ 4] >= PRINTABLE_MIN && line[ 4] <= PRINTABLE_MAX) ? line[ 4] : '.',
                (line[ 5] >= PRINTABLE_MIN && line[ 5] <= PRINTABLE_MAX) ? line[ 5] : '.',
                (line[ 6] >= PRINTABLE_MIN && line[ 6] <= PRINTABLE_MAX) ? line[ 6] : '.',
                (line[ 7] >= PRINTABLE_MIN && line[ 7] <= PRINTABLE_MAX) ? line[ 7] : '.',
                (line[ 8] >= PRINTABLE_MIN && line[ 8] <= PRINTABLE_MAX) ? line[ 8] : '.',
                (line[ 9] >= PRINTABLE_MIN && line[ 9] <= PRINTABLE_MAX) ? line[ 9] : '.',
                (line[10] >= PRINTABLE_MIN && line[10] <= PRINTABLE_MAX) ? line[10] : '.',
                (line[11] >= PRINTABLE_MIN && line[11] <= PRINTABLE_MAX) ? line[11] : '.',
                (line[12] >= PRINTABLE_MIN && line[12] <= PRINTABLE_MAX) ? line[12] : '.',
                (line[13] >= PRINTABLE_MIN && line[13] <= PRINTABLE_MAX) ? line[13] : '.',
                (line[14] >= PRINTABLE_MIN && line[14] <= PRINTABLE_MAX) ? line[14] : '.',
                (line[15] >= PRINTABLE_MIN && line[15] <= PRINTABLE_MAX) ? line[15] : '.'
                );
    }


    
    fclose(input_file);
    return 0;
}
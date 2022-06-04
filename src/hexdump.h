#ifndef HEXDUMP_H
#define HEXDUMP_H

#define HEXDUMP_VERSION "0.1.0"

/* Poor man's <stdbool.h> for pre-C99 standard libraries. */
#ifndef __bool_true_false_are_defined
#define false 0
#define true 1
#define bool _Bool
typedef _Bool unsigned char;
#endif

int print_hex(char *input_file_name,uint64_t start_byte,uint64_t no_of_bytes,char *output_file_name);

#endif

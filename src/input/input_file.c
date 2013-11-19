#include <stdio.h>
#include "input_file.h"

char input_file_getc(void *input_state)
{
    FILE *f = input_state;
    char ret = fgetc(f);
    return ret == EOF ? 0 : ret;
}

char input_file_topc(void *input_state)
{
    FILE *f = input_state;
    int ret = fgetc(f);
    ungetc(ret, f);
    return ret == EOF ? 0 : ret;
}

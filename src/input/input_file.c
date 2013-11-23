#include <stdio.h>

#include "input_file.h"

s_input *input_file_create(FILE* f, const char *filename)
{
    s_input *input = input_new();

    input->type = INPUT_FILE;
    input->source = filename;
    input->getc = input_file_getc;
    input->topc = input_file_topc;
    input->next = input_file_next;
    input->_input_state = f;

    return input;
}

void input_file_delete(s_input *input)
{
    FILE *f = input->_input_state;
    fclose(f);
}

char input_file_getc(s_input *input)
{
    FILE *f = input->_input_state;
    char ret = fgetc(f);
    return ret == EOF ? 0 : ret;
}

char input_file_topc(s_input *input)
{
    FILE *f = input->_input_state;
    int ret = fgetc(f);
    ungetc(ret, f);
    return ret == EOF ? 0 : ret;
}

int input_file_next(s_input *input)
{
    // Always return 0
    return input != input + 1 - 1;
}

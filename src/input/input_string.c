#include "input_private.h"
#include "input_string.h"

#include "string_utils.h"

s_input *input_string_create(s_string *str, const char *source)
{
    s_input *input = input_new();
    input->type = INPUT_STRING;
    input->source = source;
    input->_input_state = str;
    input->getc = input_string_getc;
    input->topc = input_string_topc;
}

char input_string_getc(s_input *input)
{
    s_string *str = input->_input_state;
    return string_getc(str);
}

char input_string_topc(s_input *input)
{
    s_string *str = input->_input_state;
    return string_topc(str);
}

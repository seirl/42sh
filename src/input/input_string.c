#include "string_utils.h"
#include "input_string.h"

char input_string_getc(void *input_state)
{
    s_string *str = input_state;
    return string_getc(str);
}

char input_string_topc(void *input_state)
{
    s_string *str = input_state;
    return string_topc(str);
}

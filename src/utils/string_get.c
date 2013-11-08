#include <stdlib.h>
#include "string_utils.h"

char string_getc(s_string *s)
{
    if (s->len == s->read_pos)
        return 0;
    return s->buf[s->read_pos++];
}

void string_rewind(s_string *s)
{
    s->read_pos = 0;
}

void string_ungetc(s_string *s)
{
    if (s->read_pos > 0)
        s->read_pos--;
}


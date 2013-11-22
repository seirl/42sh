#include <stdlib.h>
#include "string_utils.h"

char string_getc(s_string *s)
{
    if (s->len == s->read_pos)
        return 0;
    return s->buf[s->read_pos++];
}

char string_topc(s_string *s)
{
    return s->buf[s->read_pos];
}

char string_topc_n(s_string *s, unsigned n)
{
    if (s->read_pos + n < s->len)
        return s->buf[s->read_pos + n];
    return 0;
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

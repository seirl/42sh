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

void string_eat_spaces(s_string *s)
{
    char c;
    while ((c = string_getc(s)) == ' ' && c != 0)
        continue;
    if (c != 0)
        string_ungetc(s);
}

int string_eat_pattern(s_string *s, char *pattern)
{
    int i;
    for (i = 0; pattern[i] && s->buf[s->read_pos]; ++i)
    {
        if (pattern[i] != s->buf[s->read_pos])
            break;
        s->read_pos += 1;
    }
    if (pattern[i] != 0)
    {
        s->read_pos -= i;
        return 0;
    }
    return 1;
}

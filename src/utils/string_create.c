#include <stdlib.h>
#include "string_utils.h"

s_string *string_create(size_t size)
{
    s_string *s;

    if ((s = malloc(sizeof (s_string))) == NULL)
        return NULL;
    s->max_len = size ? size : 8;
    s->len = 0;
    s->buf = malloc(s->max_len);
    s->buf[0] = 0;
    return s;
}

s_string *string_create_from(char *str)
{
    s_string *s = string_create(32);
    for (int i = 0; str[i]; ++i)
        string_putc(s, str[i]);
    return s;
}

void string_resize(s_string *s)
{
    s->max_len *= 2;
    s->buf = realloc(s->buf, s->max_len);
}

char *string_release(s_string *s)
{
    char *ret = s->buf;
    free(s);
    return ret;
}

void string_free(s_string *s)
{
    free(s->buf);
    free(s);
}

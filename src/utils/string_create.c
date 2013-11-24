#include <stdlib.h>
#include <string.h>

#include "string_utils.h"
#include "smalloc.h"

s_string *string_create(size_t size)
{
    s_string *s;

    if ((s = smalloc(sizeof (s_string))) == NULL)
        return NULL;
    s->max_len = size ? size : 8;
    s->len = 0;
    s->read_pos = 0;
    s->buf = smalloc(s->max_len);
    s->buf[0] = 0;
    return s;
}

s_string *string_create_from(const char *str)
{
    s_string *s = string_create(32);
    if (str == NULL)
        return s;
    for (int i = 0; str[i]; ++i)
        string_putc(s, str[i]);
    return s;
}

void string_resize(s_string *s)
{
    s->max_len *= 2;
    s->buf = srealloc(s->buf, s->max_len);
}

s_string *string_duplicate(const s_string *src)
{
    s_string *ret = smalloc(sizeof (s_string));

    ret->buf = smalloc(src->max_len);
    memcpy(ret->buf, src->buf, src->len + 1);
    ret->len = src->len;
    ret->max_len = src->max_len;
    ret->read_pos = src->read_pos;

    return ret;
}

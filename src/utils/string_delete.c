#include "string_utils.h"
#include "smalloc.h"

void string_del_from_end(s_string *str, size_t n)
{
    while (n--)
    {
        if (str->len > 0)
        {
            str->len--;
            str->buf[str->len] = 0;
        }
    }
}

char *string_release(s_string *s)
{
    char *ret = s->buf;
    sfree(s);
    return ret;
}

void string_free(s_string *s)
{
    sfree(s->buf);
    sfree(s);
}

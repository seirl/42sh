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

void string_del_nth(s_string *str, size_t n, size_t len)
{
    size_t i;
    for (i = 0; i < n; i++)
        continue;
    for (; i + len <= str->len; i++)
        str->buf[i] = str->buf[i + len];
    str->len--;
}

char *string_release(s_string *s)
{
    char *ret = s->buf;
    sfree(s);
    return ret;
}

void string_free(s_string *s)
{
    if (!s)
        return;

    if (s->buf) /** TODO: Remove this check and fix bugs elsewhere. */
        sfree(s->buf);
    sfree(s);
}

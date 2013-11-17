#include <string.h>
#include "hashtbl.h"

unsigned long hash_string(s_string *s)
{
    return hash_char(s->buf);
}

int cmp_string(s_string *s1, s_string *s2)
{
    return strcmp(s1->buf, s2->buf);
}

unsigned long hash_char(char *s)
{
    unsigned long hash = 5381;
    int c;
    for (int i = 0; s[i]; ++i)
    {
        c = s[i];
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int cmp_char(char *s1, char *s2)
{
    return strcmp(s1, s2);
}

void null_free(void *foo)
{
    (void)foo;
}

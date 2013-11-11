#include <string.h>
#include "hashtbl.h"

unsigned long hash_string(s_string *s)
{
    unsigned long hash = 5381;
    int c;
    for (int i = 0; s->buf[i]; ++i)
    {
        c = s->buf[i];
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int cmp_string(s_string *s1, s_string *s2)
{
    return strcmp(s1->buf, s2->buf);
}

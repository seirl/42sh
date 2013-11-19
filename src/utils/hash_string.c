#include <stdlib.h>
#include <string.h>
#include "string_utils.h"
#include "hashtbl.h"

unsigned long hash_string(void *s)
{
    s_string *str = s;
    return hash_char(str->buf);
}

int cmp_string(void *s1, void *s2)
{
    s_string *str1 = s1;
    s_string *str2 = s2;
    return cmp_char(str1->buf, str2->buf);
}

void free_string(void *s)
{
    string_free(s);
}

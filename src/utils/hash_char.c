#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <stdlib.h>
#include <string.h>
#include "string_utils.h"
#include "hashtbl.h"

unsigned long hash_char(void *s)
{
    char *str = s;
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

int cmp_char(void *s1, void *s2)
{
    return strcmp(s1, s2);
}

void free_char(void *s)
{
    free(s);
}

void *dup_char(void *s)
{
    char *str = s;
    return strdup(str);
}

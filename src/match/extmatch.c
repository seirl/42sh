#ifndef _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "match.h"

static void strlower(char *str)
{
    for (unsigned i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

int optmatch(const char *pattern, const char *str, e_match_flags flags)
{
    char *ns = strdup(str);
    char *npat = strdup(pattern);
    if (flags & MATCH_NOCASE)
    {
        strlower(ns);
        strlower(npat);
    }
    int ret = 0;
    ret = my_fnmatch(npat, ns);
    free(ns);
    free(npat);
    return ret;
}

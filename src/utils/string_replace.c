#include <stdlib.h>
#include "string_utils.h"

void string_replace(s_string *s, char *pattern, char *rep)
{
    //TODO
    (void)s;
    (void)pattern;
    (void)rep;
}

void string_eat_spaces(s_string *s)
{
    char c;
    while ((c = string_getc(s)) == ' ' && c != 0)
        continue;
    if (c != 0)
        string_ungetc(s);
}

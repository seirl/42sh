#include <stdlib.h>
#include <string.h>
#include "string_utils.h"

unsigned int string_search(s_string *s, const char *needle)
{
    return strstr(s->buf, needle) - s->buf;
}

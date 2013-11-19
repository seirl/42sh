#include <ast.h>
#include <string_utils.h>


/**
** @brief 2.6.2 - Parameter expansion
*/
s_string *parameter_expand(s_string *content)
{
    if (string_getc(content) == '#')
        return NULL;
    return NULL;
}

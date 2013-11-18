#include "ast.h"
#include "smalloc.h"

s_ast_input *ast_input_new(void)
{
    s_ast_input *ret = smalloc(sizeof (s_ast_input));

    ret->list = NULL;

    return ret;
}

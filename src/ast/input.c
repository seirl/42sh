#include "ast.h"
#include "smalloc.h"

s_ast_input *ast_input_new(void)
{
    s_ast_input *ret = smalloc(sizeof (s_ast_input));

    ret->list = NULL;

    return ret;
}

void ast_input_delete(s_ast_input *input)
{
    if (!input)
        return;
    ast_list_delete(input->list);
    sfree(input);
}

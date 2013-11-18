#include "ast.h"
#include "smalloc.h"

s_ast_for *ast_for_new(void)
{
    s_ast_for *for_n = smalloc(sizeof (s_ast_for));

    for_n->cmd_list = NULL;
    for_n->values = NULL;
    for_n->identifier = NULL;

    return for_n;
}

void ast_for_delete(s_ast_for *forr)
{
    if (!forr)
        return;
    // TODO
    sfree(forr);
}

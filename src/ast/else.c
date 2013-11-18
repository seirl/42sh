#include "ast.h"
#include "smalloc.h"

s_ast_else *ast_else_new(void)
{
    s_ast_else *myelse = smalloc(sizeof (s_ast_else));

    myelse->elif_predicate = NULL;
    myelse->elif_cmds = NULL;
    myelse->else_cmds = NULL;

    return myelse;
}

void ast_else_delete(s_ast_else *myelse)
{
    if (!myelse)
        return;

    ast_else_delete(myelse->next_else);
    ast_list_delete(myelse->elif_predicate);
    ast_list_delete(myelse->elif_cmds);
    ast_list_delete(myelse->else_cmds);

    sfree(myelse);
}

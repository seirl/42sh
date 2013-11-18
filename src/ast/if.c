#include "ast.h"
#include "smalloc.h"

s_ast_if *ast_if_new(void)
{
    s_ast_if *myif = smalloc(sizeof (s_ast_if));

    myif->elif_predicate = NULL;
    myif->elif_cmds = NULL;
    myif->else_cmds = NULL;
    myif->then_cmds = NULL;
    myif->predicate = NULL;

    return myif;
}

void ast_if_delete(s_ast_if *iff)
{
    if (!iff)
        return;

    ast_list_delete(iff->elif_cmds);
    ast_list_delete(iff->predicate);
    ast_list_delete(iff->else_cmds);
    ast_list_delete(iff->then_cmds);
    ast_list_delete(iff->elif_predicate);

    sfree(iff);
}

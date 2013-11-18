#include "ast.h"
#include "smalloc.h"

s_ast_if *ast_if_new(void)
{
    s_ast_if *myif = smalloc(sizeof (s_ast_if));

    myif->predicate = NULL;
    myif->then_cmds = NULL;
    myif->else_clause = NULL;

    return myif;
}

void ast_if_delete(s_ast_if *iff)
{
    if (!iff)
        return;

    ast_list_delete(iff->predicate);
    ast_list_delete(iff->then_cmds);
    ast_else_delete(iff->else_clause);

    sfree(iff);
}

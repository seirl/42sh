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
    // TODO
    sfree(iff);
}

#include "ast.h"
#include "smalloc.h"

s_ast_and_or *ast_and_or_new(void)
{
    s_ast_and_or *and_or = smalloc(sizeof (s_ast_and_or));

    and_or->and_or = AST_CMD_AND;
    and_or->next = NULL;
    and_or->pipeline = NULL;

    return and_or;
}

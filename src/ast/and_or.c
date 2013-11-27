#include "ast.h"
#include "smalloc.h"

s_ast_and_or *ast_and_or_new(void)
{
    s_ast_and_or *and_or = smalloc(sizeof (s_ast_and_or));

    and_or->and_or = AST_CMD_NONE;
    and_or->next = NULL;
    and_or->pipeline = NULL;

    return and_or;
}

void ast_and_or_delete(s_ast_and_or *ao)
{
    if (!ao)
        return;

    ast_and_or_delete(ao->next);
    ast_pipeline_delete(ao->pipeline);

    sfree(ao);
}

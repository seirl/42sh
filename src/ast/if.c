#include "ast.h"
#include "smalloc.h"

void ast_if_delete(s_ast_if *iff)
{
    if (!iff)
        return;
    // TODO
    sfree(iff);
}

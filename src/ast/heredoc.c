#include "ast.h"
#include "smalloc.h"


void ast_heredoc_delete(s_ast_heredoc *hd)
{
    if (!hd)
        return;

    string_free(hd->heredoc);
    sfree(hd);
}

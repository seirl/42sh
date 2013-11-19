#include "ast.h"
#include "smalloc.h"

s_ast_heredoc *ast_heredoc_new(void)
{
    s_ast_heredoc *heredoc = smalloc(sizeof (s_ast_heredoc));

    heredoc->heredoc = NULL;

    return heredoc;
}

void ast_heredoc_delete(s_ast_heredoc *hd)
{
    if (!hd)
        return;

    string_free(hd->heredoc);
    sfree(hd);
}

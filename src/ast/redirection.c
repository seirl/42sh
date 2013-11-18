#include "ast.h"
#include "smalloc.h"

s_ast_redirection_list *ast_redirection_new(void)
{
    s_ast_redirection_list *redir = smalloc(sizeof (s_ast_redirection_list));

    redir->word = NULL;
    redir->io = NULL;
    redir->type = 0;
    redir->heredoc = NULL;
    redir->heredoc_delim = NULL;
    redir->next = NULL;

    return redir;
}

void ast_redirection_list_delete(s_ast_redirection_list *redir)
{
    if (!redir)
        return;

    ast_redirection_list_delete(redir->next);
    ast_compound_word_delete(redir->heredoc_delim);
    ast_heredoc_delete(redir->heredoc);
    ast_io_number_delete(redir->io);
    ast_compound_word_delete(redir->word);

    sfree(redir);
}

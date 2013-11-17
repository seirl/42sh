#include <assert.h>

#include "parser_private.h"
#include "parser_macros.h"
#include "smalloc.h"

static int is_heredoc(const s_ast_redirection *redir)
{
    return (redir && (redir->type == REDIR_HEREDOC
                      || redir->type == REDIR_HEREDOC_STRIP)
            && redir->heredoc_delim);
}

static int prefix_heredoc(s_ast_prefix *pref, s_ast_redirection **redir)
{
    if (!pref)
        return 0;

    if (is_heredoc(pref->redirection))
    {
        *redir = pref->redirection;
        return 1;
    }
    else
        return prefix_heredoc(pref->next, redir);
}

static int element_heredoc(s_ast_element *elem, s_ast_redirection **redir)
{
    if (!elem)
        return 0;

    if (is_heredoc(elem->redirection))
    {
        *redir = elem->redirection;
        return 1;
    }
    else
        return element_heredoc(elem->next, redir);
}

static int simple_command_heredoc(s_ast_simple_cmd *cmd,
                                  s_ast_redirection **redir)
{
    if (prefix_heredoc(cmd->prefixes, redir)
        || element_heredoc(cmd->elements, redir))
        return 1;
    return 0;
}

static int redirections_heredoc(s_ast_redirection_list *redirs,
                                s_ast_redirection **redir)
{
    if (!redirs)
        return 0;

    if (is_heredoc(&(redirs->redir)))
    {
        *redir = &(redirs->redir);
        return 1;
    }
    else
        return redirections_heredoc(redirs->next, redir);
}

static int command_heredoc(s_ast_cmd *cmd, s_ast_redirection **redir)
{
    if (redirections_heredoc(cmd->redirections, redir))
        return 1;
    else if (simple_command_heredoc(cmd->simple_cmd, redir))
        return 1;
    else
        return 0;
}

int maybe_parse_heredoc(s_parser *parser, s_ast_cmd *cmd)
{
    s_token *tok;
    s_ast_redirection *redir = NULL;

    if (!command_heredoc(cmd, &redir))
        return 1; /** No heredocuments. */

    if (redir->type == REDIR_HEREDOC
        && !(tok = lex_heredoc(parser->lexer, redir->heredoc_delim)))
        RETURN_PARSE_EXPECTED_INT(parser, redir->heredoc_delim->buf);
    else if (redir->type == REDIR_HEREDOC_STRIP
            && !(tok = lex_heredoc_strip(parser->lexer, redir->heredoc_delim)))
        RETURN_PARSE_EXPECTED_INT(parser, redir->heredoc_delim->buf);
    else
        assert("Well, no.");

    s_ast_heredoc *heredoc = smalloc(sizeof (s_ast_heredoc));
    heredoc->heredoc = string_duplicate(tok->value.str);
    token_free(tok);

    redir->heredoc = heredoc;
    string_free(redir->heredoc_delim);
    redir->heredoc_delim = NULL;

    return 1;
}

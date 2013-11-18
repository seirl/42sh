#include "parser_private.h"
#include "parser_macros.h"
#include "string_utils.h"

static int parse_redirection_type(s_parser *parser, e_ast_redirection_type *redir)
{
    s_token *tok = lex_look_token(parser->lexer);

#define X(Token, Ast)               \
    if (tok->type == Token)         \
    {                               \
        *redir = Ast;               \
        parser_shift_token(parser); \
        return 1;                   \
    }
#include "redirections.def"
#undef X

    return 0;
}

static s_ast_io_number *parse_io_number(s_parser *parser)
{
    s_token *tok;

    tok = lex_look_token(parser->lexer);
    if (tok->type != T_IO_NUMBER)
    {
        token_free(tok);
        return NULL;
    }

    s_ast_io_number *io = ast_io_number_new();
    io->io_number = tok->value.integer;
    token_free(tok);
    parser_shift_token(parser);

    return io;
}

static int check_heredoc_delim(s_ast_compound_word *word)
{
    if (!word)
        return 1;

    if (word->word->kind != WORD)
        return 0;

    return check_heredoc_delim(word->next);
}

static s_ast_compound_word *parse_heredoc_delim(s_parser *parser)
{
    s_ast_compound_word *word = parse_compound_word(parser);

    if (!word || !check_heredoc_delim(word))
        RETURN_PARSE_EXPECTED(parser, "here-document");

    return word;
}

s_ast_redirection_list *parse_rule_redirection(s_parser *parser)
{
    s_ast_io_number *io = parse_io_number(parser);
    s_ast_compound_word *heredoc_delim = NULL;
    s_ast_compound_word *word = NULL;

    e_ast_redirection_type redir_type;
    if (!parse_redirection_type(parser, &redir_type))
        return NULL;

    if (redir_type == REDIR_HEREDOC || redir_type == REDIR_HEREDOC_STRIP)
        heredoc_delim = parse_heredoc_delim(parser);

    s_ast_redirection_list *redirection = ast_redirection_new();
    redirection->io = io;
    redirection->word = word;
    redirection->heredoc_delim = heredoc_delim;
    redirection->type = redir_type;

    return redirection;
}

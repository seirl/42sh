#include "parser_private.h"
#include "parser_macros.h"

static void pipeline_heredoc(s_parser *parser, s_ast_pipeline *pipeline)
{
    if (!pipeline)
        return;
    maybe_parse_heredoc(parser, pipeline->cmd);
    pipeline_heredoc(parser, pipeline->next);
}

static void and_or_heredoc(s_parser *parser, s_ast_and_or *ao)
{
    if (!ao)
        return;
    pipeline_heredoc(parser, ao->pipeline);
    and_or_heredoc(parser, ao->next);
}

static void list_heredoc_here(s_parser *parser, s_ast_list *list)
{
    if (!list)
        return;
    and_or_heredoc(parser, list->and_or);
    list_heredoc_here(parser, list->next);
}

static s_ast_input *parse_rule_input_rec(s_parser *parser)
{
    s_ast_input *ast = ast_input_new();
    ast->list = parse_rule_list(parser);

    s_token *tok = lex_token(parser->lexer);
    if (!(tok->type == T_NEWLINE
        || tok->type == T_EOF))
    {
        ast_input_delete(ast);
        RETURN_PARSE_UNEXPECTED(parser, tok);
    }

    if (tok->type == T_NEWLINE)
        list_heredoc_here(parser, ast->list);

    token_free(tok);

    return ast;
}

s_ast_input *parse_rule_input(s_parser *parser)
{
    e_token_type tok = lex_look_token_type(parser->lexer);
    if (tok == T_NEWLINE)
    {
        parser_shift_token(parser);
        return ast_input_new(); /** a newline is a valid input */
    }
    else if (tok == T_EOF)
        return NULL;

    return parse_rule_input_rec(parser);
}

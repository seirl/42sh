#include "parser_private.h"
#include "log.h"

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
    s_ast_list *list;
    if (!(list = parse_rule_list(parser)))
        return NULL; /** Could not parse */
    s_ast_input *ast = ast_input_new();
    ast->list = list;

    s_token *tok = lex_token(parser->lexer);
    if (!(tok->type == T_NEWLINE
        || tok->type == T_EOF))
        LOG(ERROR, "parser: unexpected 'input' token.", NULL);

    if (tok->type == T_NEWLINE)
        list_heredoc_here(parser, ast->list);

    token_free(tok);

    return ast;
}

s_ast_input *parse_rule_input(s_parser *parser)
{
    s_token *tok = lex_look_token(parser->lexer);
    if (tok->type == T_NEWLINE)
    {
        token_free(tok);
        parser_shift_token(parser);
        return ast_input_new(); /** a newline is a valid input */
    }
    else if (tok->type == T_EOF)
    {
        token_free(tok);
        return NULL;
    }
    token_free(tok);

    return parse_rule_input_rec(parser);
}

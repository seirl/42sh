#include "parser_private.h"
#include "parser_macros.h"
#include "smalloc.h"

static s_ast_pipeline *pipeline_new(void)
{
    s_ast_pipeline *pipeline = smalloc(sizeof (s_ast_pipeline));

    pipeline->inverted = 0;
    pipeline->cmd = NULL;
    pipeline->next = NULL;

    return pipeline;
}

s_ast_pipeline *parse_rule_pipeline(s_parser *parser, int first)
{
    int inverted = 0;
    s_token *tok = lex_look_token(parser->lexer);
    if (tok->type == T_BANG)
    {
        if (!first)
            RETURN_PARSE_UNEXPECTED(tok);
        inverted = 1;
        parser_shift_token(parser);
    }
    token_free(tok);

    s_ast_cmd *cmd;
    if (!(cmd = parse_rule_command(parser)))
        return NULL;

    s_ast_pipeline *pipeline = pipeline_new();
    pipeline->cmd = cmd;
    pipeline->inverted = inverted;

    tok = lex_look_token(parser->lexer);
    if (tok->type == T_PIPE)
    {
        parser_shift_token(parser);
        if (!(pipeline->next = parse_rule_pipeline(parser, 0)))
            return NULL;
    }
    token_free(tok);

    return pipeline;
}

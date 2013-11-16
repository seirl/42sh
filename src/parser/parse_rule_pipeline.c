#include "parser_private.h"
#include "smalloc.h"

static s_ast_pipeline *pipeline_new(void)
{
    s_ast_pipeline *pipeline = smalloc(sizeof (s_ast_pipeline));

    pipeline->inverted = 0;
    pipeline->cmd = NULL;
    pipeline->next = NULL;

    return pipeline;
}

s_ast_pipeline *parse_rule_pipeline(s_parser *parser)
{
    int inverted = 0;
    s_token *tok = lex_look_token(parser->lexer);
    if (tok->type == T_BANG)
    {
        inverted = 1;
        token_free(lex_token(parser->lexer));
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
        pipeline->next = parse_rule_pipeline(parser);
        parser_shift_token(parser);
    }
    token_free(tok);

    return pipeline;
}

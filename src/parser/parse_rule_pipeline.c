#include "parser_private.h"
#include "parser_macros.h"

static int parse_bang(s_parser *parser, int first)
{
    int inverted = 0;

    s_token *tok = lex_look_token(parser->lexer);
    if (tok->type == T_BANG)
    {
        if (!first)
            RETURN_PARSE_UNEXPECTED_INT(parser, tok);
        inverted = 1;
        parser_shift_token(parser);
    }
    token_free(tok);

    return inverted;
}

static s_ast_pipeline *sub_parse_rule_pipeline(s_parser *parser,
                                               s_ast_pipeline *pipeline)
{
    parser_shift_token(parser); /** shift | */

    parse_heredoc_here(parser, pipeline->cmd);

    if (!(pipeline->next = parse_rule_pipeline(parser, 0)))
        return NULL;
    return pipeline;
}

s_ast_pipeline *parse_rule_pipeline(s_parser *parser, int first)
{
    s_token *tok;
    int inverted = parse_bang(parser, first);

    s_ast_cmd *cmd;
    if (!(cmd = parse_rule_command(parser)))
        return NULL;

    s_ast_pipeline *pipeline = ast_pipeline_new();
    pipeline->cmd = cmd;
    pipeline->inverted = inverted;

    tok = lex_look_token(parser->lexer);
    if (tok->type == T_PIPE)
        sub_parse_rule_pipeline(parser, pipeline);
    token_free(tok);
    return pipeline;
}

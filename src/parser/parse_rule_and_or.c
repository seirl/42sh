#include "parser_private.h"
#include "token.h"

s_ast_and_or *parse_rule_and_or(s_parser *parser)
{
    s_ast_pipeline *pipeline;
    if (!(pipeline = parse_rule_pipeline(parser, 1)))
        return NULL;

    s_ast_and_or *and_or = ast_and_or_new();
    and_or->pipeline = pipeline;

    s_token *tok = lex_look_token(parser->lexer);
    switch (tok->type)
    {
    case T_AND_IF: /** && */
        and_or->and_or = AST_CMD_AND;
        break;
    case T_OR_IF: /** || */
        and_or->and_or = AST_CMD_OR;
        break;
    default:
        token_free(tok);
        return and_or;
    }
    token_free(tok);
    parser_shift_token(parser);

    parse_heredoc_here(parser, pipeline->cmd);

    and_or->next = parse_rule_and_or(parser);

    return and_or;
}

#include "parser_private.h"
#include "parser_macros.h"
#include "token.h"

static s_ast_and_or *parse_rule_and_or_rec(s_parser *parser, s_ast_and_or *ao)
{
    e_token_type type = lex_look_token_type(parser->lexer);
    switch (type)
    {
    case T_AND_IF: /** && */
        ao->and_or = AST_CMD_AND;
        break;
    case T_OR_IF: /** || */
        ao->and_or = AST_CMD_OR;
        break;
    default:
        ao->and_or = AST_CMD_NONE;
        return ao;
    }
    parser_shift_token(parser);

    parse_heredoc_here(parser, ao->pipeline->cmd);

    if (!(ao->next = parse_rule_and_or(parser)))
    {
        ast_and_or_delete(ao);
        RETURN_PARSE_EXPECTED(parser, "after && or ||");
    }

    return ao;
}

s_ast_and_or *parse_rule_and_or(s_parser *parser)
{
    s_ast_pipeline *pipeline;
    if (!(pipeline = parse_rule_pipeline(parser, 1)))
        return NULL;

    s_ast_and_or *and_or = ast_and_or_new();
    and_or->pipeline = pipeline;

    return parse_rule_and_or_rec(parser, and_or);
}

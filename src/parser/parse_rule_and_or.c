#include "parser_private.h"
#include "token.h"
#include "smalloc.h"

static s_ast_and_or *and_or_new(void)
{
    s_ast_and_or *and_or = smalloc(sizeof (s_ast_and_or));

    and_or->and_or = AST_CMD_AND;
    and_or->next = NULL;
    and_or->pipeline = NULL;

    return and_or;
}

s_ast_and_or *parse_rule_and_or(s_parser *parser)
{
    s_ast_pipeline *pipeline;
    if (!(pipeline = parse_rule_pipeline(parser)))
        return NULL;

    s_ast_and_or *and_or = and_or_new();
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

    parse_expect_newlines(parser);

    and_or->next = parse_rule_and_or(parser);

    return and_or;
}

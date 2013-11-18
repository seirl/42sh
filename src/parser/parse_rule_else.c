#include "parser_private.h"
#include "parser_macros.h"

static s_ast_else *parse_elif(s_parser *parser)
{
    s_ast_else *else_clause = ast_else_new();

    parser_shift_token(parser);
    if (!(else_clause->elif_predicate = parse_rule_compound_list(parser, 1)))
        return NULL;

    parse_expect_token(parser, T_THEN);

    if (!(else_clause->elif_cmds = parse_rule_compound_list(parser, 1)))
        return NULL;

    else_clause->next_else = parse_rule_else(parser);

    return else_clause;
}

static s_ast_else *parse_else(s_parser *parser)
{
    s_ast_else *else_clause = ast_else_new();

    parser_shift_token(parser);
    if (!(else_clause->else_cmds = parse_rule_compound_list(parser, 1)))
        return NULL;

    return else_clause;
}

s_ast_else *parse_rule_else(s_parser *parser)
{
    s_token *tok;
    s_ast_else *else_clause = NULL;

    tok = lex_look_token(parser->lexer);
    if (tok->type == T_ELIF)
        else_clause = parse_elif(parser);
    else if (tok->type == T_ELSE)
        else_clause = parse_else(parser);
    token_free(tok);

    return else_clause;
}

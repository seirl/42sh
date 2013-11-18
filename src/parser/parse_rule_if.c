#include "parser_private.h"
#include "parser_macros.h"

static int parse_rule_else(s_parser *parser,
                           s_ast_list **elif_predicate,
                           s_ast_list **elif_cmds,
                           s_ast_list **else_cmds)
{
    s_token *tok;

    tok = lex_look_token(parser->lexer);
    if (tok->type == T_ELIF)
    {
        parser_shift_token(parser);
        if (!(*elif_predicate = parse_rule_compound_list(parser, 1)))
            return 0;
        parse_expect_token(parser, T_THEN);
        if (!(*elif_cmds = parse_rule_compound_list(parser, 1)))
            return 0;
    }
    token_free(tok);

    tok = lex_look_token(parser->lexer);
    if (tok->type == T_ELSE)
    {
        parser_shift_token(parser);
        if (!(*else_cmds = parse_rule_compound_list(parser, 1)))
            return 0;
    }
    token_free(tok);

    return 1;
}

s_ast_if *parse_rule_if(s_parser *parser)
{
    s_ast_list *predicate = NULL;
    s_ast_list *elif_predicate = NULL;
    s_ast_list *elif_cmds = NULL;
    s_ast_list *else_cmds = NULL;

    if (!(predicate = parse_rule_compound_list(parser, 1)))
        RETURN_PARSE_EXPECTED(parser, "predicate");

    if (!parse_expect_token(parser, T_THEN))
        RETURN_PARSE_EXPECTED(parser, "then");

    s_ast_list *then_cmds = parse_rule_compound_list(parser, 1);

    if (!parse_rule_else(parser, &elif_predicate, &elif_cmds, &else_cmds))
        RETURN_PARSE_EXPECTED(parser, "well formated else/elif");

    if (!parse_expect_token(parser, T_FI))
        RETURN_PARSE_EXPECTED(parser, "fi");

    s_ast_if *myif = ast_if_new();
    myif->predicate = predicate;
    myif->then_cmds = then_cmds;
    myif->elif_predicate = elif_predicate;
    myif->elif_cmds = elif_cmds;
    myif->else_cmds = else_cmds;

    return myif;
}

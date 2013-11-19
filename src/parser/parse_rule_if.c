#include "parser_private.h"
#include "parser_macros.h"

s_ast_if *parse_rule_if(s_parser *parser)
{
    s_ast_list *predicate = NULL;

    if (!(predicate = parse_rule_compound_list(parser, 1)))
        RETURN_PARSE_EXPECTED(parser, "predicate");

    if (!parse_expect_token(parser, T_THEN))
        RETURN_PARSE_EXPECTED(parser, "then");

    s_ast_list *then_cmds = parse_rule_compound_list(parser, 1);

    s_ast_else *else_clause = parse_rule_else(parser);

    if (!parse_expect_token(parser, T_FI))
        RETURN_PARSE_EXPECTED(parser, "fi");

    s_ast_if *myif = ast_if_new();

    myif->predicate = predicate;
    myif->then_cmds = then_cmds;
    myif->else_clause = else_clause;

    return myif;
}

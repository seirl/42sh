#include "parser_private.h"
#include "parser_macros.h"

s_ast_list *parse_rule_do_group(s_parser *parser)
{
    if (!parse_expect_token(parser, T_DO))
        RETURN_PARSE_EXPECTED(parser, "do");

    s_ast_list *list = parse_rule_compound_list(parser, 1);

    if (!parse_expect_token(parser, T_DONE))
    {
        ast_list_delete(list);
        RETURN_PARSE_EXPECTED(parser, "done");
    }

    return list;
}

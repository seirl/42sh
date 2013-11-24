#include "parser_private.h"
#include "ast.h"

s_ast_case_item *parse_rule_case_clause(s_parser *parser)
{
    s_ast_case_item *item = parse_rule_case_item(parser);

    if (item)
    {
        parse_expect_newlines(parser);
        e_token_type type = lex_look_token_type(parser->lexer);
        if (type == T_DSEMI)
            parser_shift_token(parser);
        parse_expect_newlines(parser);
        type = lex_look_token_type(parser->lexer);
        if (type != T_ESAC)
            item->next = parse_rule_case_clause(parser);
    }

    return item;
}

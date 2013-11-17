#include "parser_private.h"
#include "parser_macros.h"

s_ast_list *parse_rule_compound_list(s_parser *parser, int first)
{
    s_ast_and_or *and_or;
    s_ast_list *list;
    s_token *tok;

    parse_expect_newlines(parser);

    if (!(and_or = parse_rule_and_or(parser)))
    {
        if (first)
            RETURN_PARSE_EXPECTED(parser, "and_or rule");
        else
            return NULL;
    }

    list = ast_list_new();
    list->and_or = and_or;

    tok = lex_look_token(parser->lexer);
    if (tok->type == T_AND || tok->type == T_SEMI || tok->type == T_NEWLINE)
    {
        parser_shift_token(parser);
        list->next_asynchronous = tok->type == T_AND;
        list->next = parse_rule_compound_list(parser, 0);
    }
    token_free(tok);

    return list;
}

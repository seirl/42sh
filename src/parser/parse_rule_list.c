#include "parser_private.h"

s_ast_list *parse_rule_list(s_parser *parser)
{
    s_ast_and_or *sub;
    if (!(sub = parse_rule_and_or(parser)))
        return NULL; /** Could not parse */

    s_ast_list *list = ast_list_new();
    list->and_or = sub;

    s_token *tok = lex_look_token(parser->lexer);
    switch (tok->type)
    {
    case T_AND:
        list->next_asynchronous = 1;
    case T_SEMI:
        parser_shift_token(parser);
        list->next = parse_rule_list(parser);
    default:
        break;
    }
    token_free(tok);

    return list;
}

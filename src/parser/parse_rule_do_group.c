#include "parser_private.h"
#include "parser_macros.h"

s_ast_list *parse_rule_do_group(s_parser *parser)
{
    s_token *tok;

    tok = lex_token(parser->lexer);
    if (!tok->type == T_DO)
        RETURN_PARSE_EXPECTED(parser, "do");
    token_free(tok);

    s_ast_list *list = parse_rule_compound_list(parser, 1);

    tok = lex_token(parser->lexer);
    if (!tok->type == T_DONE)
        RETURN_PARSE_EXPECTED(parser, "done");
    token_free(tok);

    return list;
}

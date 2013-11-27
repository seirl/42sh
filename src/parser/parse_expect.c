#include "parser_private.h"
#include "parser_macros.h"

void parse_expect_newlines(s_parser *parser)
{
    while (lex_look_token_type(parser->lexer) == T_NEWLINE)
        parser_shift_token(parser);
}

int parse_expect_token(s_parser *parser, e_token_type type)
{
    e_token_type tok = lex_look_token_type(parser->lexer);
    if (tok == type)
    {
        parser_shift_token(parser);
        return 1;
    }
    return 0;
}

int parse_expect_newline_or_semi(s_parser *parser)
{
    e_token_type tok = lex_look_token_type(parser->lexer);
    if (tok == T_NEWLINE || tok == T_SEMI)
    {
        parser_shift_token(parser);
        return 1;
    }
    return 0;
}

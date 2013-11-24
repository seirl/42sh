#include "parser_private.h"
#include "parser_macros.h"

void parse_expect_newlines(s_parser *parser) // TODO use look_type
{
    s_token *tok;

    while ((tok = lex_look_token(parser->lexer)))
    {
        if (tok->type != T_NEWLINE)
        {
            token_free(tok);
            return;
        }
        token_free(tok);
        token_free(lex_token(parser->lexer)); // TODO: use shift
    }
}

int parse_expect_token(s_parser *parser, e_token_type type)
{
    s_token *tok = lex_look_token(parser->lexer); // TODO use look_type
    if (tok->type == type)
        parser_shift_token(parser);
    else
    {
        token_free(tok);
        return 0;
    }
    token_free(tok);
    return 1;
}

int parse_expect_newline_or_semi(s_parser *parser)
{
    s_token *tok;
    tok = lex_token(parser->lexer); // TODO use look_type
    if (tok->type == T_NEWLINE || tok->type == T_SEMI)
    {
        token_free(tok);
        return 1;
    }
    token_free(tok);
    return 0;
}

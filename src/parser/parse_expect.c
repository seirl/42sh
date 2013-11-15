#include "parser_private.h"

void parse_expect_newlines(s_parser *parser)
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
        token_free(lex_token(parser->lexer));
    }
}

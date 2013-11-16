#include "parser_private.h"
#include "token.h"
#include "smalloc.h"

s_ast_word *word_of_token(s_token *tok)
{
    s_ast_word *word = smalloc(sizeof (s_ast_word));

    word->str = string_duplicate(tok->value.str);

    return word;
}

void parser_shift_token(s_parser *parser)
{
    token_free(lex_token(parser->lexer));
}

void parser_shift_word(s_parser *parser)
{
    token_free(lex_word(parser->lexer));
}

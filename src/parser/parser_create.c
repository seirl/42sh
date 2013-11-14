#include "parser_private.h"
#include "smalloc.h"

s_parser *parser_create(s_lexer *lexer)
{
    s_parser *parser;

    parser = smalloc(sizeof (s_parser));
    parser->lexer = lexer;

    return parser;
}

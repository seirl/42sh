#include "parser_private.h"
#include "smalloc.h"

void parser_delete(s_parser *parser)
{
    lex_delete(parser->lexer);
    sfree(parser);
}

#include "parser_private.h"
#include "smalloc.h"

void parser_delete(s_parser *parser)
{
    sfree(parser->lexer);
    sfree(parser);
}

#include "parser_private.h"
#include "lexer.h"

int parser_ready(s_parser *parser)
{
    // Get a new sequence of chars
    return lex_start(parser->lexer);
}

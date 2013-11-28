#include "parser_private.h"
#include "lexer.h"
#include "log.h"

e_parser_status parser_diagnostic(s_parser *parser)
{
    if (parser->status == PARSE_ERROR
        || lex_error(parser->lexer) == E_LEX_UNBALANCED)
        return PARSE_ERROR;
    return PARSE_OK;
}

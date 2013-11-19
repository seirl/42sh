#include "parser_private.h"
#include "parser_macros.h"
#include "log.h"

int parser_diagnostic(s_parser *parser)
{
    if (!parser_eof(parser))
    {
        s_token *tok = lex_look_token(parser->lexer);
        RETURN_PARSE_UNEXPECTED_INT(parser, tok);
    }
    else if (parser->status == PARSE_ERROR)
    {
        LOG(WARN, "Parse error", NULL);
        return 0;
    }
    return 1;
}

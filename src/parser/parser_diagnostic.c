#include "parser_private.h"
#include "log.h"

e_parser_status parser_diagnostic(s_parser *parser)
{
    if (parser->status == PARSE_ERROR)
    {
        LOG(WARN, "Parse error", NULL);
        return PARSE_ERROR;
    }
    return PARSE_OK;
}

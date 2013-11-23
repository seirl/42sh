#include "parser_private.h"
#include "log.h"

int parser_diagnostic(s_parser *parser)
{
    if (parser->status == PARSE_ERROR)
    {
        LOG(WARN, "Parse error", NULL);
        return 0;
    }
    return 1;
}

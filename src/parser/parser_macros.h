#ifndef PARSER_MACROS
# define PARSER_MACROS

# include "log.h"

# define RETURN_PARSE_UNEXPECTED(Token)                     \
    do {                                                    \
        LOG(ERROR, "Unexpected %s", Token->value.str->buf); \
        return NULL;                                        \
    } while (0);

#endif /* !PARSER_MACROS */

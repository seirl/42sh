#ifndef PARSER_MACROS
# define PARSER_MACROS

# include "log.h"

# define RETURN_PARSE_EXPECTED(Parser, What) \
    do {                                     \
        Parser->status = PARSE_ERROR;        \
        LOG(ERROR, "Expected %s", What);     \
        return NULL;                         \
    } while (0)

# define RETURN_PARSE_UNEXPECTED(Parser, Token)             \
    do {                                                    \
        Parser->status = PARSE_ERROR;                       \
        LOG(ERROR, "Unexpected %s", Token->value.str->buf); \
        return NULL;                                        \
    } while (0)

# define RETURN_PARSE_EXPECTED_INT(Parser, Str) \
    do {                                        \
        Parser->status = PARSE_ERROR;           \
        LOG(ERROR, "Expected %s", Str);         \
        return 0;                               \
    } while (0)

# define RETURN_PARSE_UNEXPECTED_INT(Parser, Token)         \
    do {                                                    \
        Parser->status = PARSE_ERROR;                       \
        LOG(ERROR, "Unexpected %s", Token->value.str->buf); \
        return 0;                                           \
    } while (0)

#endif /* !PARSER_MACROS */

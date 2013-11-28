#include <string.h>
#include "lexer_private.h"

int is_operator(char c)
{
    return (c == '&' || c == '|' || c == ';' || c == '<' || c == '>'
           || c == '!' || c == '}' || c == '{' || c == '(' || c == ')');
}

int is_delimiter(s_lexer *lexer, char c)
{
    int ret = (c == 0
               || (lexer->context == LEX_ALL
                  && (c == '\t' || c == ' ' || c == '\n' || (is_operator(c)))
                  && (c != '!' || lexer->working_buffer->len == 0)));
    if (lexer->assignment && (c == '{' || c == '}'))
        return 0;
    return ret;
}

int is_quote(char c)
{
    if (c == '\'')
        return 1;
    if (c == '\"')
        return 2;
    if (c == '`')
        return 4;
    return 0;
}

int is_token_delimiter(s_lexer *lexer, char c)
{
    return is_delimiter(lexer, c) || is_quote(c);
}

int is_valid_operator(s_lexer *lexer, s_string *s)
{
#define X(Tok, Str)                                         \
    if (!strcmp(s->buf, Str))                               \
    {                                                       \
        lexer->token_type = Tok;                            \
        lexer->concat = Tok != T_BANG ? -1 : 0;             \
        return 1;                                           \
    }
#include "operator.def"
#undef X
    return 0;
}

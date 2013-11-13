#include "char_utils.h"

int is_delimiter(char c)
{
    return (c == 0 || c == ' ' || c == '&' || c == '|' || c == ';' || c == '<'
           || c == '>' || c == '(' || c == ')');
}

int is_quote(char c)
{
    return c == '\'' || c == '\"' || c == '`';
}

int is_token_delimiter(char c)
{
    return is_delimiter(c) || is_quote(c);
}

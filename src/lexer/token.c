#include <stdio.h>
#include "token.h"

void token_print(s_token *tok)
{
#define X(Type, Str)                            \
    if (tok->type == Type)                      \
    {                                           \
        fprintf(stdout, "%s", ##Type);          \
        if (Str[0] != 0)                        \
            fprintf(stdout, " (%s)", Str);      \
        break;                                  \
    }
#include "token.def"
#undef X
    if (tok->value)
        fprintf(stdout, "with value: %s", tok->value->buf);
    fprintf(stdout, "\n");
}

s_token *token_create(e_type type, s_string *value)
{
    s_token *tok = smalloc(sizeof (s_token));
    tok->type = type;
    tok->value = value;
    return tok;
}

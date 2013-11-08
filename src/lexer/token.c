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

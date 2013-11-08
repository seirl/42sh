#include <stdio.h>
#include "token.h"

void token_print(s_token *tok)
{
#define X(Type, Str) \
    if (tok->type == Type) \
    { \
        fprintf(stdout, "%s", ##Type);\
        break;\
    }
#include "token.def"
#undef X
    if (tok->value)
        fprintf(stdout, "(%s)", tok->value->buf);
    fprintf(stdout, "\n");
}

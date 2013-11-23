#include <stdio.h>

#include "token.h"

void token_print(s_token *tok)
{
    fprintf(stdout, "%s", tok->value.str->buf);
    do {
#define X(Type, Str)                            \
    if (tok->type == Type)                      \
    {                                           \
        fprintf(stdout, "\t"#Type);             \
        break;                                  \
    }
#include "misc.def"
#include "res_word.def"
#include "operator.def"
#undef X
    } while (0);
    fprintf(stdout, " (%d)\n", tok->concat == -1 ? 0 : tok->concat);
}

#include "token.h"

int token_is_res_word(s_token *tok)
{
#define X(Type, Str)                \
    if (tok->type == Type)          \
        return 1;
#include "res_word.def"
#undef X
    return 0;
}

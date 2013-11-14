#include <stdio.h>
#include "token.h"
#include "smalloc.h"

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
    fprintf(stdout, "\n");
}

s_token *token_create(e_token_type type,
                      u_token_value value,
                      s_location location)
{
    s_token *tok;

    tok = smalloc(sizeof (s_token));

    tok->type = type;
    tok->value = value;
    tok->location = location;
    tok->blank_preceed = 0;

    return tok;
}

void token_free(s_token *tok)
{
    if (tok->type == T_WORD) // FIXME: Other types may hold strings
        string_free(tok->value.str);
    sfree(tok);
}

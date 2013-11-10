#include <stdio.h>
#include "token.h"
#include "smalloc.h"

void token_print(s_token *tok)
{
    do {
#define X(Type, Str)                            \
    if (tok->type == Type)                      \
    {                                           \
        fprintf(stdout, "%s ", Str);            \
        break;                                  \
    }
#include "misc.def"
#include "res_word.def"
#include "operator.def"
#undef X
    } while (0);
    if (tok->type == T_WORD)
        fprintf(stdout, "with value: %s",
                string_nullterminated(tok->value.str));
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

    return tok;
}

void token_free(s_token *tok)
{
    if (tok->type == T_WORD) // FIXME: Other types may hold strings
        string_free(tok->value.str);
    sfree(tok);
}

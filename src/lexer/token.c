#include <stdio.h>

#include "token.h"
#include "smalloc.h"
#include "string_utils.h"

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
                      s_location location,
                      int blank_preceed)
{
    s_token *tok;

    tok = smalloc(sizeof (s_token));

    tok->type = type;
    tok->value = value;
    tok->location = location;
    tok->blank_preceed = blank_preceed;

    return tok;
}

s_token *token_duplicate(const s_token *tok)
{
    if (tok->type == T_WORD)
    {
        u_token_value value;
        value.str = string_duplicate(tok->value.str);
        return (token_create(tok->type, value,
               tok->location, tok->blank_preceed));
    }
    return (token_create(tok->type, tok->value, tok->location,
           tok->blank_preceed));
}

void token_free(s_token *tok)
{
    // FIXME: Other types may hold strings
    if (tok->type == T_WORD && tok->value.str)
        string_free(tok->value.str);
    sfree(tok);
}

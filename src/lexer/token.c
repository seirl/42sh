#include "token.h"
#include "smalloc.h"
#include "string_utils.h"

s_token *token_create(e_token_type type,
                      s_token_value value,
                      s_location location,
                      int concat)
{
    s_token *tok;

    tok = smalloc(sizeof (s_token));

    tok->aliasable = 0;
    tok->concat = concat;
    tok->location = location;
    tok->type = type;
    tok->value = value;

    return tok;
}

s_token *token_duplicate(const s_token *tok)
{
    s_token_value value;
    value.str = string_duplicate(tok->value.str);
    value.integer = tok->value.integer;
    return (token_create(tok->type, value, tok->location, tok->concat));
}

void token_free(s_token *tok)
{
    if (!tok)
        return;
    if (tok->value.str)
        string_free(tok->value.str);
    sfree(tok);
}

int token_to_word(s_token *token)
{
#define X(Type)               \
    if (token->type == Type)  \
        return 1;
#include "token2word.def"
#undef X
#define X(Type, Str)          \
    if (token->type == Type)  \
    {                         \
        token->type = T_WORD; \
        return 1;             \
    }
#include "res_word.def"
#undef X
    if (token->type == T_ASSIGNMENT_WORD)
    {
        token->type = T_WORD;
        return 1;
    }
    return 0;
}

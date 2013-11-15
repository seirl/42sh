#include "lexer_private.h"

//! @brief Replace s with new s_string, return old value.
static s_string *string_moult(s_string **s)
{
    s_string *ret = *s;

    *s = string_create(0);

    return ret;
}

static void lexer_reset(s_lexer *lexer)
{
    lexer->token_type = T_WORD;
    lexer->concat = 0;
    string_reset(lexer->working_buffer);
}

s_token *lex_release_token(s_lexer *lexer)
{
    s_token *tok;
    e_token_type type = T_WORD;
    u_token_value value =
    {
        NULL
    };

    type = lexer->token_type;
    value.str = string_moult(&(lexer->working_buffer));
    tok = token_create(type, value, lexer->location, 0);

    lexer_reset(lexer);

    return tok;
}

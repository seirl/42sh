#include "lexer_private.h"

s_token *lex_release_token(s_lexer *lexer)
{
    s_token *ret;
    e_token_type type = T_WORD;
    u_token_value value =
    {
        NULL
    };

    if (lexer->token_type != T_WORD)
    {
        type = lexer->token_type;
        lexer->token_type = T_WORD;
        string_reset(lexer->working_buffer);
    }
    else
    {
        value.str = lexer->working_buffer;
        lexer->working_buffer = string_create(0);
    }

    ret = token_create(type, value, lexer->location);

    return ret;
}

#include <stdlib.h>
#include "lexer.h"
#include "lexer_private.h"
#include "smalloc.h"

s_lexer *lex_create(char (*lex_getc)(void *input_state),
                    char (*lex_topc)(void *input_state),
                    void *input_state,
                    char *source)
{
    s_lexer *lexer;

    lexer = smalloc(sizeof (s_lexer));

    lexer->getc = lex_getc;
    lexer->topc = lex_topc;
    lexer->source = source;
    lexer->working_buffer = string_create(0);
    lexer->token_type = T_WORD;
    lexer->sur.begin = 0;
    lexer->sur.end = 0;
    lexer->sur.count = 0;
    lexer->quoted = 0;
    lexer->concat = -1;
    lexer->lookahead = NULL;
    lexer->prefill = 1;
    lexer->input_state = input_state;

    return lexer;
}

void lex_delete(s_lexer *lexer)
{
    string_free(lexer->working_buffer);
    if (lexer->lookahead)
        token_free(lexer->lookahead);
    sfree(lexer);
}

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
    string_reset(lexer->working_buffer);
}

s_token *lex_release_token(s_lexer *lexer)
{
    s_token *tok;
    e_token_type type = T_WORD;
    s_token_value value =
    {
        NULL,
        0
    };

    type = lexer->token_type;
    value.str = string_moult(&(lexer->working_buffer));
    value.integer = type == T_IO_NUMBER ? atoi(value.str->buf) : 0;
    tok = token_create(type, value, lexer->location, lexer->concat);

    lexer_reset(lexer);

    return tok;
}

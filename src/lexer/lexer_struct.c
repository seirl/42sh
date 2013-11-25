#include <stdlib.h>
#include "lexer.h"
#include "lexer_private.h"
#include "input_private.h"
#include "smalloc.h"
#include "shopt.h"

s_lexer *lex_create(s_shell *shell, s_input *input, int blank_sep)
{
    s_lexer *lexer;

    lexer = smalloc(sizeof (s_lexer));

    lexer->input = input;

    lexer->working_buffer = string_create(0);
    lexer->token_type = T_WORD;
    lexer->sur.begin = 0;
    lexer->sur.end = 0;
    lexer->sur.count = 0;
    lexer->quoted = 0;
    lexer->concat = -1;
    lexer->lookahead = NULL;
    lexer->prefill = 1;
    lexer->shell = shell;
    lexer->blank_sep = blank_sep;

    return lexer;
}

int lex_start(s_lexer *lexer)
{
    token_free(lexer->lookahead);
    lexer->lookahead = NULL;
    if (lexer->working_buffer->len == 0)
        lexer->prefill = 1;
    return lexer->input->next(lexer->input) || lexer->working_buffer->len != 0;
}

void lex_delete(s_lexer *lexer)
{
    string_free(lexer->working_buffer);
    if (lexer->lookahead)
        token_free(lexer->lookahead);

    input_destroy(lexer->input);
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
    char *end = NULL;
    e_token_type type = T_WORD;
    s_token_value value =
    {
        NULL,
        -2
    };

    type = lexer->token_type;
    value.str = string_moult(&(lexer->working_buffer));
    if (type == T_IO_NUMBER)
    {
        int io = strtol(value.str->buf, &end, 10);
        value.integer = !*end ? io : -2;
    }
    value.integer = type == T_IO_NUMBER ? atoi(value.str->buf) : -2;
    tok = token_create(type, value, lexer->location, lexer->concat);
    tok->aliasable = update_alias_delimiter(lexer);

    lexer_reset(lexer);
    if (lexer->shell && shopt_get(lexer->shell, "token_print"))
        token_print(tok);
    expand_alias(lexer->shell, tok);
    return tok;
}

#include <stdio.h>
#include <ctype.h>

#include "lexer.h"
#include "lexer_private.h"
#include "location.h"

#undef getc
s_token *lex_word(s_lexer *lexer)
{
    s_token *tok = lex_look_token(lexer);

    if (token_to_word(tok))
    {
        token_free(tok);
        tok = lex_token(lexer);
        token_to_word(tok);
        if (tok->type == T_WORD && lexer->working_buffer->buf[0] == '(')
            tok->type = T_FUNCTION_NAME;
        return tok;
    }

    token_free(tok);
    return NULL;
}

s_token *lex_name(s_lexer *lexer)
{
    s_token *tok = lex_look_token(lexer);

    if (tok->type == T_ASSIGNMENT_WORD)
    {
        token_free(tok);
        tok = lex_token(lexer);
        return tok;
    }
    token_free(tok);
    return NULL;
}

static void check_prefill(s_lexer *lexer)
{
    if (lexer->prefill)
    {
        fill_token(lexer);
        lexer->prefill = 0;
    }
}

s_token *lex_token(s_lexer *lexer)
{
    if (lexer->lookahead)
        return lex_release_lookahead(lexer);

    check_prefill(lexer);

    do {
        if (handle_eof(lexer))
            break;
        if (handle_newline(lexer))
            break;
        if (handle_name(lexer))
            break;
        if (lexer->context == LEX_ALL && handle_res_word(lexer))
            break;
        if (lexer->context == LEX_ALL && handle_io_number(lexer))
            break;
    } while (0);

    s_token *ret = lex_release_token(lexer);
    fill_token(lexer);

    if (ret->concat == -1)
        ret->concat = 0;
    strip_token(ret);
    remove_backslash(lexer, ret);
    return ret;
}

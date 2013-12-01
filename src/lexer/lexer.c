#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "lexer_private.h"
#include "input_private.h"
#include "location.h"
#include "string_utils.h"

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
    s_token *tok = lex_token(lexer);

#define X(Type, Str)        \
    if (tok->type == Type)  \
    {                       \
        tok->type = T_WORD; \
        return tok;         \
    }
#include "res_word.def"
#undef X

    if (tok->type == T_WORD && is_valid_name(tok->value.str))
        return tok;
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

static int escaped_newline(s_lexer *lexer, s_token *tok)
{
    if (lexer->input->type != INPUT_INTERACTIVE)
        return 0;
    if (lex_topc(lexer) == 0
       && lexer->context == LEX_ALL
       && tok->value.str->buf[tok->value.str->len - 2] == '\\'
       && tok->value.str->buf[tok->value.str->len - 3] != '\\')
    {
        string_del_from_end(tok->value.str, 2);
        lexer->input->next(lexer->input, "PS2");
        lex_getc(lexer);
        lexer->concat = -1;
        fill_token(lexer);
        if (tok->value.str->len == 0)
        {
            token_free(tok);
            return 1;
        }
    }
    return 0;
}

static void update_type(s_lexer *lexer, s_token *tok)
{
    lexer->assignment = strcmp(tok->value.str->buf, "=") == 0;
}

static s_token *lex_token_real(s_lexer *lexer)
{
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
    update_type(lexer, ret);
    fill_token(lexer);

    if (ret->concat == -1)
        ret->concat = 0;
    if (escaped_newline(lexer, ret))
        return lex_token(lexer);
    strip_token(ret);
    remove_backslash(lexer, ret);
    return ret;
}

s_token *lex_token(s_lexer *lexer)
{
    if (lexer->lookahead)
        return lex_release_lookahead(lexer);

    s_token *ret;
    if (lexer->sublexer)
    {
        if ((ret = lex_token(lexer->sublexer))->type != T_EOF)
            return ret;
        else
        {
            input_destroy(lexer->sublexer->input);
            lex_delete(lexer->sublexer);
            lexer->sublexer = NULL;
        }
    }

    return lex_token_real(lexer);
}

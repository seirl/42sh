#include <stdio.h>
#include <ctype.h>

#include "lexer.h"
#include "lexer_private.h"
#include "char_utils.h"
#include "location.h"

static int lex_res_word(s_lexer *lexer)
{
#define X(Type, Str)                              \
    if (string_equal(lexer->working_buffer, Str)) \
    {                                             \
        lexer->token_type = Type;                 \
        lexer->concat = 0;                       \
        return 1;                                 \
    }
#include "res_word.def"
#undef X
    return 0;
}

static int lex_io_number(s_lexer *lexer)
{
    for (size_t i = 0; i < lexer->working_buffer->len; ++i)
    {
        if (!isdigit(lexer->working_buffer->buf[i]))
            return 0;
    }
    if (lexer->topc(lexer) == '>' || lexer->topc(lexer) == '<')
    {
        lexer->token_type = T_IO_NUMBER;
        return 1;
    }
    return 0;
}

#if 0
static int lex_assignment(s_lexer *lexer)
{
    for (size_t i = 0; i < lexer->working_buffer->len - 1; ++i)
    {
        char c = lexer->working_buffer->buf[i];
        if (c == '\'' || c == '\"')
            return 0;
        if (c == '=' && i != 0)
        {
            lexer->token_type = T_ASSIGNMENT_WORD;
            return 1;
        }
    }
    return 0;
}
#endif

static int lex_name(s_lexer *lexer)
{
    if (lexer->token_type == T_WORD && lexer->working_buffer->buf[0] == '$')
    {
        if (lexer->working_buffer->buf[1] == '('
           && lexer->working_buffer->buf[2] == '(')
            return 0;
        lexer->token_type = T_NAME;
        return 1;
    }
    return 0;
}

static int lex_eof(s_lexer *lexer)
{
    if (lexer->working_buffer->len == 0)
    {
        lexer->token_type = T_EOF;
        lexer->concat = 0;
        return 1;
    }
    return 0;
}

static int lex_newline(s_lexer *lexer)
{
    if (lexer->working_buffer->buf[0] == '\n')
    {
        lexer->token_type = T_NEWLINE;
        lexer->concat = -1;
        return 1;
    }
    return 0;
}

static int token_to_word(s_token *token)
{
#define X(Type, Str)                              \
    if (token->type == Type)                      \
    {                                             \
        token->type = T_WORD;                     \
        return 1;                                 \
    }
#include "res_word.def"
#undef X
    if (token->type == T_ASSIGNMENT_WORD
       || token->type == T_IO_NUMBER
       || token->type == T_WORD)
    {
        token->type = T_WORD;
        return 1;
    }
    return 0;
}

s_token *lex_word(s_lexer *lexer)
{
    s_token *tok = lex_look_token(lexer);

    if (token_to_word(tok))
    {
        token_free(tok);
        return lex_token(lexer);
    }

    token_free(tok);
    return NULL;
}

s_token *lex_token(s_lexer *lexer)
{
    if (lexer->lookahead)
        return lex_release_lookahead(lexer);

    lex_delimit_token(lexer);

    do {
        if (lex_eof(lexer))
            break;
        if (lex_newline(lexer))
            break;
        if (lex_name(lexer))
            break;
        if (lex_res_word(lexer))
            break;
        if (lex_io_number(lexer))
            break;
    } while (0);
#if 0
    if (lex_assignment(lexer))
        return lex_release_token(lexer);
#endif
    s_token *ret = lex_release_token(lexer);
    if (ret->concat == -1)
        ret->concat = 0;
    return ret;
}

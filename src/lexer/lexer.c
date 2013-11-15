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

static int lex_eof(s_lexer *lexer)
{
    if (lexer->working_buffer->len == 0)
    {
        lexer->token_type = T_EOF;
        return 1;
    }
    return 0;
}

static int lex_newline(s_lexer *lexer)
{
    if (lexer->working_buffer->buf[0] == '\n')
    {
        lexer->token_type = T_NEWLINE;
        return 1;
    }
    return 0;
}

s_token *lex_token(s_lexer *lexer)
{
    lex_delimit_token(lexer);

    if (lex_eof(lexer))
        return lex_release_token(lexer);
    if (lex_newline(lexer))
        return lex_release_token(lexer);
    if (lex_res_word(lexer))
        return lex_release_token(lexer);
    if (lex_io_number(lexer))
        return lex_release_token(lexer);
    if (0 && lex_assignment(lexer))
        return lex_release_token(lexer);
    return lex_release_token(lexer);
}

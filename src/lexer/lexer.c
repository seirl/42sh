#include "lexer.h"
#include "lexer_private.h"
#include "char_utils.h"
#include "location.h"

static int lex_eof(s_lexer *lexer)
{
    char c = string_topc(lexer->working_buffer);
    if (c == '\0')
    {
        // Don't eat the EOF
        lexer->token_type = T_EOF;
        return 1;
    }
    return 0;
}

static int lex_newline(s_lexer *lexer)
{
    if (string_topc(lexer->working_buffer) == '\n')
    {
        lexer->token_type = T_NEWLINE;
        location_next_line(&(lexer->location));
        return 1;
    }
    return 0;
}

static int lex_comment(s_lexer *lexer)
{
    if (string_topc(lexer->working_buffer) == '#')
    {
        char c;
        while ((c = string_getc(lexer->working_buffer)) != '\n')
        {
            if (c == 0 && lex_fill_buf(lexer))
                return 1;
        }
        return 1;
    }
    return 0;
}

static int lex_operator(s_lexer *lexer)
{
#define X(Type, Str)                              \
    if (string_equal(lexer->working_buffer, Str)) \
    {                                             \
        lexer->token_type = Type;                 \
        return 1;                                 \
    }
#include "operator.def"
#undef X
    return 0;
}

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
    // TODO
    (void) lexer;
    return 0;
}

// FIXME: correct word definition
s_token *lex_word(s_lexer *lexer)
{
    char c;
    while ((c = string_getc(lexer->working_buffer)) != 0)
    {
        if (is_quote(c))
        {
            
        }
    }
    return lex_release_token(lexer);
}

#include <stdio.h>
s_token *lex_token(s_lexer *lexer)
{
    if (lexer->working_buffer->len == 0)
        lex_fill_buf(lexer);

    // 2.3 1. If the end of input is recognized, the current token shall be
    // delimited. If there is no current token, the end-of-input indicator
    // shall be returned as the token.
    if (lex_eof(lexer))
        return lex_release_token(lexer);

    // 2.3 10. If the current character is a '#', it and all subsequent
    // characters up to, but excluding, the next <newline> shall be discarded
    // as a comment.  The <newline> that ends the line is not considered part
    // of the comment.
    if (lex_comment(lexer))
        return lex_token(lexer);

    // 2.10.1 1. A <newline> shall be returned as the token identifier NEWLINE.
    if (lex_newline(lexer))
        return lex_release_token(lexer);

    if (lex_res_word(lexer))
        return lex_release_token(lexer);

    // 2.10.1 2. If the token is an operator, the token identifier for that
    // operator shall result.
    if (lex_operator(lexer))
        return lex_release_token(lexer);

    // 2.10.1 3. If the string consists solely of digits and the delimiter
    // character is one of '<' or '>', the token identifier IO_NUMBER shall be
    // returned.
    if (lex_io_number(lexer))
        return lex_release_token(lexer);

    // 2.10.1 4.Otherwise, the token identifier TOKEN results.
    return lex_word(lexer);
}

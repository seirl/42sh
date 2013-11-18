#include <ctype.h>
#include "lexer_private.h"

int handle_res_word(s_lexer *lexer)
{
#define X(Type, Str)                              \
    if (string_equal(lexer->working_buffer, Str)) \
    {                                             \
        lexer->token_type = Type;                 \
        lexer->concat = 0;                        \
        return 1;                                 \
    }
#include "res_word.def"
#undef X
    return 0;
}

int handle_io_number(s_lexer *lexer)
{
    for (size_t i = 0; i < lexer->working_buffer->len; ++i)
    {
        if (!isdigit(lexer->working_buffer->buf[i]))
            return 0;
    }
    if (lexer->topc(lexer->input_state) == '>'
       || lexer->topc(lexer->input_state) == '<')
    {
        lexer->token_type = T_IO_NUMBER;
        return 1;
    }
    return 0;
}

int handle_name(s_lexer *lexer)
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

int handle_eof(s_lexer *lexer)
{
    if (lexer->working_buffer->len == 0)
    {
        lexer->token_type = T_EOF;
        lexer->concat = 0;
        return 1;
    }
    return 0;
}

int handle_newline(s_lexer *lexer)
{
    if (lexer->working_buffer->buf[0] == '\n')
    {
        lexer->token_type = T_NEWLINE;
        lexer->concat = -1;
        return 1;
    }
    return 0;
}

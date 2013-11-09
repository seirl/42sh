#include "lexer.h"

static int lex_eof(s_string *stream, s_token **tok)
{
    char c = string_getc(stream);
    if (c == 0)
        *tok = token_create(T_EOF, NULL);
    else
        string_ungetc(stream);
    return c == 0;
}

static int lex_newline(s_string *stream, s_token **tok)
{
    if (string_eat_pattern(stream, "\n"))
    {
        *tok = token_create(T_NEWLINE, NULL);
        return 1;
    }
    return 0;
}

static int lex_comment(s_string *stream, s_token **tok)
{
    char c = string_getc(stream);
    if (c == '#')
    {
        while ((c = string_getc(stream)) != '\n' && c != 0)
            continue;
        *tok = token_create(T_NEWLINE, NULL);
        string_ungetc(stream);
        return 1;
    }
    string_ungetc(stream);
    return 0;
}

static int lex_operator(s_string *stream, s_token **tok)
{
#define X(Type, Str)                          \
    if (string_eat_pattern(stream, Str))      \
    {                                         \
        *tok = token_create(Type, NULL);      \
        return 1;                             \
    }
#include "operator.def"
#undef X
    return 0;
}

static s_token *next_token(s_string *stream)
{
    s_token *ret;
    string_eat_spaces(stream);
    if (lex_eof(stream, &ret))
        return ret;
    if (lex_comment(stream, &ret))
        return ret;
    if (lex_newline(stream, &ret))
        return ret;
    if (lex_operator(stream, &ret))
        return ret;
    return ret;
}

s_token_queue *lex(char *str)
{
    s_string *stream = string_create_from(str);
    s_token *tok;
    s_token_queue *q = token_queue_create();
    do
    {
        tok = next_token(stream);
        token_enqueue(q, tok);
    } while (tok->type != T_EOF);
    string_free(stream);
    return NULL;
}

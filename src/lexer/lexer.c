#include "lexer.h"

static int lex_eof(s_string *stream, s_token **tok)
{
    char c = string_getc(stream);
    if (c == 0)
        *tok = token_create(T_EOF, NULL);
    else
        string_ungetc(stream);
}

static int lex_newline(s_string *stream, s_token **tok)
{
    char c = string_getc(stream);
    if (c == '\n')
        *tok = token_create(T_NEWLINE, NULL);
    else
        string_ungetc(stream);
}

static int lex_comment(s_string *stream, s_token **tok)
{
    char c = string_getc(stream);
    if (c == '#')
    {
        while (c = string_getc(stream) != '\n' && c != 0)
            continue;
        *tok = token_create(T_NEWLINE, NULL);
    }
    string_ungetc(stream);
}

static s_token *next_token(s_string *stream)
{
    s_token *ret;
    string_eat_spaces(stream);
    char c;
}

void lex(char *str)
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
}

#include "string_utils.h"
#include "lexer_private.h"

int check_delim(s_lexer *lexer, const s_string *delim)
{
    int index;
    char c;

    index = lexer->working_buffer->len - delim->len - 1;
    c = string_index(lexer->working_buffer, index);
    if ((index == -1 || (c && c == '\n'))
        && string_ends_with(lexer->working_buffer, delim))
    {
        lexer->working_buffer->read_pos = 0;
        s_string *tmp = string_extract(lexer->working_buffer, 0,
                                       lexer->working_buffer->len
                                       - delim->len);
        string_free(lexer->working_buffer);
        lexer->working_buffer = tmp;
        return 1;
    }
    return 0;
}

static s_token *heredoc_preprocess(s_lexer *lexer, const s_string *delim)
{
    if (check_delim(lexer, delim))
    {
        lexer->token_type = T_WORD;
        return lex_release_token(lexer);
    }

    return NULL;
}

s_token *lex_heredoc(s_lexer *lexer, const s_string *delim)
{
    s_token *tok;
    if ((tok = heredoc_preprocess(lexer, delim)))
        return tok;
    char c = 0;
    while (1)
    {
        if ((c = lex_topc(lexer)) == '\0') /** EOF */
            return NULL;
        if (c == '\n' && check_delim(lexer, delim))
        {
            lex_getc(lexer);
            break;
        }

        string_putc(lexer->working_buffer, lex_getc(lexer));
    }
    lexer->token_type = T_WORD;
    return lex_release_token(lexer);
}

s_token *lex_heredoc_strip(s_lexer *lexer, const s_string *delim)
{
    s_token *tok;
    if ((tok = heredoc_preprocess(lexer, delim)))
        return tok;
    char c = 0;
    char last = '\n';
    while (1)
    {
        if ((c = lex_topc(lexer)) == '\0') /** EOF */
            return NULL;
        if ((last == '\n' || last == '\t') && c == '\t')
        {
            last = c;
            lex_getc(lexer);
            continue;
        }
        last = c;

        if (c == '\n' && check_delim(lexer, delim))
        {
            lex_getc(lexer);
            break;
        }
        string_putc(lexer->working_buffer, lex_getc(lexer));
    }
    lexer->token_type = T_WORD;
    return lex_release_token(lexer);
}

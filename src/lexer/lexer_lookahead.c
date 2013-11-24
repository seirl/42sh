#include <assert.h>

#include "lexer_private.h"
#include "token.h"

s_token *lex_look_token(s_lexer *lexer)
{
    if (lexer->lookahead)
        return token_duplicate(lexer->lookahead);

    lexer->lookahead = lex_token(lexer);
    return token_duplicate(lexer->lookahead);
}

e_token_type lex_look_token_type(s_lexer *lexer)
{
    if (lexer->lookahead)
        return lexer->lookahead->type;

    lexer->lookahead = lex_token(lexer);
    return lexer->lookahead->type;
}

s_token *lex_look_word(s_lexer *lexer)
{
    s_token *tok = lex_look_token(lexer);

    if (token_to_word(tok))
    {
        if (tok->type == T_WORD && lexer->working_buffer->buf[0] == '(')
            tok->type = T_FUNCTION_NAME;

        return tok;
    }

    token_free(tok);
    return NULL;
}

s_token *lex_look_name(s_lexer *lexer)
{
    s_token *tok = lex_look_token(lexer);

    if (tok->type == T_ASSIGNMENT_WORD)
        return tok;

    token_free(tok);
    return NULL;
}

s_token *lex_release_lookahead(s_lexer *lexer)
{
    assert(lexer->lookahead);
    s_token *tok = lexer->lookahead;
    lexer->lookahead = NULL;
    return tok;
}

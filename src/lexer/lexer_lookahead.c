#include <assert.h>

#include "lexer_private.h"

s_token *lex_look_token(s_lexer *lexer)
{
    if (lexer->lookahead)
        return token_duplicate(lexer->lookahead);

    lexer->lookahead = lex_token(lexer);
    return token_duplicate(lexer->lookahead);
}

s_token *lex_look_word(s_lexer *lexer)
{
    if (lexer->lookahead)
        return token_duplicate(lexer->lookahead);

    lexer->lookahead = lex_word(lexer);
    if (lexer->lookahead)
        return token_duplicate(lexer->lookahead);
    else
        return NULL;
}

s_token *lex_release_lookahead(s_lexer *lexer)
{
    assert(lexer->lookahead);
    s_token *tok = lexer->lookahead;
    lexer->lookahead = NULL;
    return tok;
}

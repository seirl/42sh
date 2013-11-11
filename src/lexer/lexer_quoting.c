#include "lexer_private.h"

int lex_single_quotes(s_lexer *lexer)
{
    if (lex_topc(lexer) == '\'')
    {
        // Quotes are in the token
        lex_eat(lexer);
        char c;
        while ((c = lex_topc(lexer)) && c != '\'')
            lex_eat(lexer);

        // Quotes are in the token
        if (c == '\'')
            lex_eat(lexer);
        return 1;
    }
    return 0;
}

void lex_double_quotes_piece(s_lexer *lexer)
{
    char c;
    while ((c = lex_topc(lexer)) && c != '"')
    {
        if (c == '$')
        {
            lexer->token_type = T_DOUBLE_QUOTES_PIECE;
            return;
        }
        lex_eat(lexer);
    }
    // End of double quotes
    if (c == '"')
        lexer->token_type = T_DOUBLE_QUOTES;
    else
    {
        // FIXME: lex error?
        ;
    }
}

int lex_double_quotes(s_lexer *lexer)
{
    if (lex_topc(lexer) == '"')
    {
        lexer->token_type = T_DOUBLE_QUOTES;
        lex_double_quotes_piece(lexer);
        return 1;
    }
    return 0;
}


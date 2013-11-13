#ifndef LEXER_PRIVATE_H
# define LEXER_PRIVATE_H

# include "lexer.h"
# include "token.h"
# include "location.h"

/**
** @brief The internal state of the lexer.
*/
struct lexer
{
    char (*getc)(void *input_state);
    char (*topc)(void *input_state);
    void *input_state;
    //! Where does the characters come from.
    char *source;
    e_token_type token_type;
    //! Is the current character quoted (lose special meaning)?
    int quoted;
    //! Have a blank character preceeded the token?
    int blank;
    s_string *working_buffer;
    s_location location;
};

/**
** @brief Lookup a char from input stream.
*/
char lex_topc(s_lexer *lexer);

/**
** @brief Take a char from input stream.
*/
char lex_getc(s_lexer *lexer);

/**
** @brief Discard char from input stream.
*/
void lex_discard(s_lexer *lexer);

/**
** @brief Take a char from input stream and put it in the working buffer.
*/
void lex_eat(s_lexer *lexer);

/**
** @brief Lex up to next single quote.
**
** cf. 2.2.2 Single-Quotes.
*/
int lex_single_quotes(s_lexer *lexer);

/**
** @brief Lex up to next double quote or special character.
**
** If no special character is seen this function will ask to return a
** T_DOUBLE_QUOTE.
**
** If a '$' is seen this function will ask to return T_DOUBLE_QUOTE_PIECE,
** asking the parser to ask the lexer back for a token, and so on, up to a
** T_RPAREN (end of expansion) and then ask the lexer resume the double quote
** lexing.  The '$' is not eat if it is seen.
**
** cf. 2.2.3 Double-Quotes.
*/
int lex_double_quotes(s_lexer *lexer);

/**
** @brief Produce a token from current lexer state.
**
** The lexer working buffer is transfered to the produced token and a new one
** is put in the lexer.
**
** The token is said to be delimited.
*/
s_token *lex_release_token(s_lexer *lexer);

int lex_fill_buf(s_lexer *lexer);

#endif /* !LEXER_PRIVATE_H */

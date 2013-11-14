#ifndef LEXER_PRIVATE_H
# define LEXER_PRIVATE_H

# include "lexer.h"
# include "token.h"
# include "location.h"

struct surround
{
    char begin;
    char end;
    int count;
};
typedef struct surround s_surround;

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
    s_string *working_buffer;
    s_location location;
    s_surround sur;
    char quoted;
    int concat;
};

/**
** @brief Produce a token from current lexer state.
**
** The lexer working buffer is transfered to the produced token and a new one
** is put in the lexer.
**
** The token is said to be delimited.
*/
s_token *lex_release_token(s_lexer *lexer);

int lex_delimit_token(s_lexer *lexer);

#endif /* !LEXER_PRIVATE_H */

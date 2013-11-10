#ifndef LEXER_H
# define LEXER_H

# include "string_utils.h"
# include "token.h"

typedef struct lexer s_lexer;

/**
** @brief Create a new lexer.
**
** @param lex_getc A function that return a char
** @return A lexer object.
*/
s_lexer *lex_create(char (*lex_getc)(void *input_state),
                    char (*lex_topc)(void *input_state),
                    char *source);

/**
** @brief Lex a token or a word.
**
** @return A token.
*/
s_token *lex_token(s_lexer *lex);

/**
** @brief Lex a word.
**
** @return A word token.
*/
s_token *lex_word(s_lexer *lex);

#endif /* !LEXER_H */

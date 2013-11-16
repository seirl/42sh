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
** @brief Delete a lexer.
*/
void lex_delete(s_lexer *lexer);

/**
** @brief Lex a token or a word.
**
** @return A token.
*/
s_token *lex_token(s_lexer *lexer);

/**
** @brief Lex a token or a word and keep it.
**
** @return A copy of the next token.
*/
s_token *lex_look_token(s_lexer *lexer);

/**
** @brief Lex a word.
**
** @return A word token or NULL if it is not a word.
*/
s_token *lex_word(s_lexer *lexer);

/**
** @brief Lex a token or a word and keep it.
**
** @return A copy of the next token.
*/
s_token *lex_look_word(s_lexer *lexer);

#endif /* !LEXER_H */

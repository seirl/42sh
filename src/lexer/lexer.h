#ifndef LEXER_H
# define LEXER_H

# include "string_utils.h"
# include "input.h"
# include "token.h"

typedef struct lexer s_lexer;

enum lexer_context
{
    LEX_ALL,
    LEX_SQUOTE,
    LEX_DQUOTE,
};
typedef enum lexer_context e_lexer_context;

/**
** @brief Create a new lexer.
**
** Get ownership the input (will destroy it).
*/
s_lexer *lex_create(s_shell *shell, s_input *input, e_lexer_context context);

/**
** @brief Start a new sequence of lexing.
**
** Call the input for a new sequence of chars and clean the lexer lookahead.
*/
int lex_start(s_lexer *lexer);

/**
** @brief Delete a lexer.
*/
void lex_delete(s_lexer *lexer);

s_token *lex_token(s_lexer *lexer);
s_token *lex_look_token(s_lexer *lexer);
e_token_type lex_look_token_type(s_lexer *lexer);

s_token *lex_word(s_lexer *lexer);
s_token *lex_look_word(s_lexer *lexer);

s_token *lex_name(s_lexer *lexer);
s_token *lex_look_name(s_lexer *lexer);

/**
** @brief Lex a heredoc.
**
** @return A heredoc token or NULL if it is not a heredoc.
*/
s_token *lex_heredoc(s_lexer *lexer, const s_string *word);

/**
** @brief Lex a stripped heredoc.
**
** @return A heredoc token or NULL if it is not a heredoc.
*/
s_token *lex_heredoc_strip(s_lexer *lexer, const s_string *word);

#endif /* !LEXER_H */

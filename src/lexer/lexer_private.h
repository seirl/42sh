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
    //! Lookahead token
    s_token *lookahead;
    //! Do we have to pre-fill the buffer?
    int prefill;
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
s_token *lex_release_lookahead(s_lexer *lexer);

int lex_delimit_token(s_lexer *lexer);
int lex_eat_spaces(s_lexer *lexer);

// char_utils.c
int is_delimiter(char c);
int is_quote(char c);
int is_token_delimiter(char c);
int is_operator(char c);
int is_valid_operator(s_lexer *lexer, s_string *s);

// lexer_pattern.c
int handle_comment(s_lexer *lexer, char c, char prev);
int handle_dollar(s_lexer *lexer, char c, char prev);
int handle_quotes(s_lexer *lexer, char c, char prev);
int handle_operator(s_lexer *lexer);
int handle_assignment(s_lexer *lexer, char c);

// lexer_word.c
int handle_newline(s_lexer *lexer);
int handle_eof(s_lexer *lexer);
int handle_name(s_lexer *lexer);
int handle_io_number(s_lexer *lexer);
int handle_res_word(s_lexer *lexer);

// lexer_fill.c
int fill_until(s_lexer *lexer, int include_last);
int fill_upto_delim(s_lexer *lexer);
int fill_token(s_lexer *lexer);

#endif /* !LEXER_PRIVATE_H */

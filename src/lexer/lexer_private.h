#ifndef LEXER_PRIVATE_H
# define LEXER_PRIVATE_H

# include "lexer.h"
# include "token.h"
# include "location.h"
# include "input.h"
# include "shell.h"

/**
** @brief Used to count the occurence of a char and his opposite
*/
struct surround
{
    //! the first char
    char begin;
    //! the expected char to exit from this state
    char end;
    //! count to handle nested char
    int count;
};
typedef struct surround s_surround;

/**
** @brief The internal state of the lexer.
*/
struct lexer
{
    s_input *input;

    e_token_type token_type;
    //! a buffer in which the char are set
    s_string *working_buffer;
    s_location location;
    //! informations about the current state (in quotes, braces, ...)
    s_surround sur;
    char quoted;
    //! is the token part of the previous?
    int concat;
    //! Lookahead token
    s_token *lookahead;
    //! Do we have to pre-fill the buffer?
    int prefill;
    //! blank char = separator ?
    int blank_sep;
    int prev_delim;
    //! Shell
    s_shell *shell;
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
/**
** @brief Show the resulting token of the lexer state
**
** The working buffer is not flushed.
**
*/
s_token *lex_release_lookahead(s_lexer *lexer);

// input utils ----------------------------------------------------------------
char lex_getc(s_lexer *lexer);
char lex_topc(s_lexer *lexer);

/** @brief fill the buffer with a new delimited token */
int lex_delimit_token(s_lexer *lexer);
/** @brief remove blank char (' ', '\t') from the input stream */
int lex_eat_spaces(s_lexer *lexer);

// char utils -----------------------------------------------------------------
/** @brief check if the current char is a delimiter according to the SCL */
int is_delimiter(s_lexer *lexer, char c);
/** @brief check if the current char is a quote (', ", `) */
int is_quote(char c);
/** @brief check if the current char is a token delimiter (ie quote, blank) */
int is_token_delimiter(s_lexer *lexer, char c);
/** @brief check if the current char is an operator */
int is_operator(char c);
/** @brief check if the wroking buffer contains an operator (SCL) */
int is_valid_operator(s_lexer *lexer, s_string *s);

// pattern recognition --------------------------------------------------------
/** @brief check if we have to deal with a comment */
int handle_comment(s_lexer *lexer, char c, char prev);
/** @brief check if the buffer begins with a $ */
int handle_dollar(s_lexer *lexer, char c, char prev);
/** @brief deal with quoted strings */
int handle_quotes(s_lexer *lexer, char c, char prev);
/** @brief set the token as an operator */
int handle_operator(s_lexer *lexer);
/** @brief check if the word is an assignment (charset + '=') */
int handle_assignment(s_lexer *lexer, char c);

// special words --------------------------------------------------------------
/** @brief check if the token is a newline */
int handle_newline(s_lexer *lexer);
/** @brief check if the token only contain EOF */
int handle_eof(s_lexer *lexer);
/** @brief check if the token is a variable name */
int handle_name(s_lexer *lexer);
/** @brief check if the token is an I/O number (file descriptor )*/
int handle_io_number(s_lexer *lexer);
/** @brief check if the token is a reserved words (SCL) */
int handle_res_word(s_lexer *lexer);

// buffer utils ---------------------------------------------------------------
/** @brief fill the working buffer until the end of a char (surround field) */
int fill_until(s_lexer *lexer, int include_last);
/** @brief fill the buffer until a delimiter appear */
int fill_upto_delim(s_lexer *lexer);
/** @brief fill the buffer with a new delimited token */
int fill_token(s_lexer *lexer);

/** @brief remove the useless part of a token */
void strip_token(s_token *token);

void expand_alias(s_shell *shell, s_token *tok);
int update_alias_delimiter(s_lexer *lexer);

#endif /* !LEXER_PRIVATE_H */

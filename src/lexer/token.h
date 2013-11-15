#ifndef TOKEN_H
# define TOKEN_H

# include "string_utils.h"
# include "location.h"

/**
 ** @brief Type of a token.
 */
enum token_type
{
# define X(Type, Str) Type,
# include "misc.def"
# include "operator.def"
# include "res_word.def"
# undef X
};
typedef enum token_type e_token_type;

/**
** @brief Content of a token (if any).
*/
union token_value
{
    s_string *str;
    long integer;
};
typedef union token_value u_token_value;

/**
** @brief A token generated by the lexer.
*/
struct token
{
    e_token_type type;
    u_token_value value;
    s_location location;
    int concat;
};
typedef struct token s_token;

/**
** @brief Create a token.
*/
s_token *token_create(e_token_type type,
                      u_token_value value,
                      s_location location,
                      int concat);

/**
** @brief Duplicate a token.
*/
s_token *token_duplicate(const s_token *token);

void token_free(s_token *tok);

// Debug functions
void token_print(s_token *tok);

#endif /* !TOKEN_H */

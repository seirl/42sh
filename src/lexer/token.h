#ifndef TOKEN_H
# define TOKEN_H

# include "string_utils.h"

enum token_type
{
# define X(Type, Str) Type,
# include "token.def"
# undef X
};
typedef enum token_type e_token_type;

struct token
{
    e_token_type type;
    s_string *value;
    struct token *next;
};
typedef struct token s_token;

void token_print(s_token *tok);

#endif /* !TOKEN_H */

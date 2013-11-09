#ifndef TOKEN_H
# define TOKEN_H

# include "string_utils.h"

enum token_type
{
# define X(Type, Str) Type,
# include "misc.def"
# include "operator.def"
# include "res_word.def"
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
s_token *token_create(e_token_type type, s_string *value);
void token_free(s_token *tok);

#endif /* !TOKEN_H */

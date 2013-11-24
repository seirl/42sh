#include <stdio.h>
#include <ctype.h>

#include "lexer.h"
#include "lexer_private.h"
#include "location.h"

void strip_token(s_token *token)
{
    if (token->type == T_NAME)
        string_del_nth(token->value.str, 0, 1);
    else if (token->type == T_PARAM_EXPANSION || token->type == T_CMD_SUBST)
    {
        string_del_from_end(token->value.str, 1);
        string_del_nth(token->value.str, 0, 2);
    }
    else if (token->type == T_ARITHM)
    {
        string_del_from_end(token->value.str, 1);
        string_del_nth(token->value.str, 0, 3);
    }
    else if (token->type == T_SQUOTE
            || token->type == T_DQUOTE
            || token->type == T_BQUOTE)
    {
        string_del_from_end(token->value.str, 1);
        string_del_nth(token->value.str, 0, 1);
    }
}

#include "string_utils.h"
#include "shell.h"
#include "expand.h"
#include "lexer_private.h"
#include "token.h"

void remove_backslash(s_lexer *lexer, s_token *tok)
{
    if (tok->type == T_DQUOTE || tok->type == T_SQUOTE
       || lexer->context == LEX_DQUOTE || lexer->context == LEX_SQUOTE)
        return;
    s_string *str = tok->value.str;
    for (int i = 0; str->buf[i]; ++i)
    {
        if (str->buf[i] == '\\')
        {
            char c = str->buf[i + 1];
            if (c != '\"')
            {
                for (int j = 0; str->buf[i + j]; ++j)
                    str->buf[i + j] = str->buf[i + j + 1];
                str->len -= 1;
            }
        }
    }
}

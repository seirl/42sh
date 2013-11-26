#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"
#include "lexer.h"
#include "token.h"
#include "input_string.h"

#if 0
static void remove_special_backslash(s_string *str)
{
    for (int i = 0; str->buf[i]; ++i)
    {
        if (str->buf[i] == '\\')
        {
            char c = str->buf[i + 1];
            if (c == '$' || c == '`' || c == '\"' || c == '\n')
            {
                for (int j = 0; str->buf[i + j]; ++j)
                    str->buf[i + j] = str->buf[i + j + 1];
            }
        }
    }
}

static s_string *expand_token(s_shell *shell, s_string *word)
{
    s_input *input = input_string_create(word, "dquote");
    s_lexer *lexer = lex_create(shell, input, LEX_DQUOTE);
    s_token *tok;
    e_token_type t;
    s_string *ret = string_create(0);

    do {
        tok = lex_token(lexer);
        t = tok->type;
        if (t == T_NAME)
        {
            s_string *sub = expand_string(shell, tok->value.str, EXPAND_NAME);
            string_cat(ret, sub);
            string_free(sub);
        }
        else
        {
            remove_special_backslash(tok->value.str);
            string_cat(ret, tok->value.str);
        }
        token_free(tok);
    } while (t != T_EOF);
    return ret;
}
#endif

s_string *expand_dquote(s_shell *shell, s_string *word)
{
    s_string *r;

    r = expand_string(shell, string_duplicate(word), LEX_DQUOTE);
    //TODO schischi
    //if (t == T_WORD)
    //    remove_special_backslash(sub);
    return r;
}

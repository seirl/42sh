#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"
#include "lexer.h"
#include "token.h"
#include "input_string.h"
#include "evalexpr.h"

#define VARNAME "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"

static int among(char c, const char *s)
{
    for (unsigned i = 0; s[i]; i++)
        if (c == s[i])
            return 1;
    return 0;
}

static s_string *extract_vars(s_shell *shell, s_string *s)
{
    s_string *n = string_create(s->len);
    for (unsigned i = 0; i < s->len; i++)
    {
        if (among(s->buf[i], VARNAME))
        {
            s_string *id = string_create(0);
            while (among(s->buf[i], VARNAME))
            {
                string_putc(id, s->buf[i]);
                i++;
            }
            // if env_get returns NULL, string_puts will ignore it.
            string_puts(n, env_get(shell, id->buf));
            string_free(id);
        }
        string_putc(n, s->buf[i]);
    }
    return n;
}

s_string *expand_arithm(s_shell *shell, s_string *word)
{
    s_string *r;
    s_string *r_vars;
    r = expand_string(shell, string_duplicate(word), LEX_DQUOTE);
    r_vars = extract_vars(shell, r);

    long result = evalexpr(r_vars->buf);
    string_free(r);
    string_free(r_vars);

    return string_itoa(result);
}

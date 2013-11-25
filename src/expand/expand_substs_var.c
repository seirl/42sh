#include <stdlib.h>
#include <string.h>

#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"

/*
** Use Alternative Value. If parameter is unset or null, null shall be
** substituted; otherwise, the expansion of word shall be substituted.
*/
static s_string *expand_alternative(const char *word, const s_string *var,
                                    int only_unset)
{
    // TODO: expand word
    if (!var || (!var->buf[0] && !only_unset))
        return string_create_from("");
    else
        return string_create_from(word);
}

/*
** Use Default Values. If parameter is unset or null, the expansion of word
** shall be substituted; otherwise, the value of parameter shall be
** substituted.
*/
static s_string *expand_default(const char *word, const s_string *var,
                                int only_unset)
{
    // TODO: expand word
    if (!var || (!var->buf[0] && !only_unset))
        return string_create_from(word);
    else
        return string_duplicate(var);
    return NULL;
}

/*
** Assign Default Values. If parameter is unset or null, the expansion of word
** shall be assigned to parameter. In all cases, the final value of parameter
** shall be substituted. Only variables, not positional parameters or special
** parameters, can be assigned in this way.
*/
static s_string *expand_assign(const char *word, const s_string *var,
                               s_shell *shell, int only_unset)
{
    (void) word;
    (void) var;
    (void) only_unset;
    (void) shell;
    //TODO
    return NULL;
}

/*
** Indicate Error if Null or Unset. If parameter is unset or null, the
** expansion of word (or a message indicating it is unset if word is omitted)
** shall be written to standard error and the shell exits with a non-zero exit
** status. Otherwise, the value of parameter shall be substituted. An
** interactive shell need not exit.
*/
static s_string *expand_error(const char *word, const s_string *var,
                              int only_unset)
{
    (void) word;
    (void) var;
    (void) only_unset;
    //TODO
    return NULL;
}

static s_string *expand_del_prefix(const char *word, const s_string *var)
{
    (void) word;
    (void) var;
    //TODO
    return NULL;
}

static s_string *expand_del_suffix(const char *word, const s_string *var)
{
    (void) word;
    (void) var;
    //TODO
    return NULL;
}

s_string *expand_substs_param(s_shell *shell, const char *param,
                              const s_string *var)
{
    int only_unset = 1;
    if (param[0] == ':')
    {
        param++;
        only_unset = 0;
    }
#define X(Op, Func, ...)                        \
    if (param[0] == (#Op)[0])                   \
        return Func(param + 1, __VA_ARGS__);
#include "expand_params.def"
#undef X
    return NULL;
}

static int among(char c, const char *s)
{
    while (*s)
        if (c == *s++)
            return 1;
    return 0;
}

s_string *expand_substs_var(s_shell *shell, const s_string *word)
{
    if (word->buf[0] == '#')
    {
        const char *r = env_get(shell, word->buf + 1);
        return string_itoa(r ? strlen(r) : 0);
    }

    s_string *varname = string_create(0);

    unsigned i;
    for (i = 0; word->buf[i] != '\0' && !among(word->buf[i], ":+-?=%#"); i++)
        string_putc(varname, word->buf[i]);

    s_string *var_content = string_create_from(env_get(shell, varname->buf));
    if (!word->buf[i])
        return var_content;

    s_string *r = expand_substs_param(shell, word->buf + i, var_content);

    free(varname);
    free(var_content);
    return r;
}

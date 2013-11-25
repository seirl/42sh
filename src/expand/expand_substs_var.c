#include <stdlib.h>
#include <string.h>

#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"

static int among(char c, const char *s)
{
    while (*s)
        if (c == *s++)
            return 1;
    return 0;
}

static s_string *expand_alternative(const char *param, const s_string *var,
                                    int only_unset)
{
    (void) param;
    (void) var;
    (void) only_unset;
    //TODO
    return NULL;
}

static s_string *expand_default(const char *param, const s_string *var,
                                int only_unset)
{
    (void) param;
    (void) var;
    (void) only_unset;
    //TODO
    return NULL;
}

static s_string *expand_assign(const char *param, const s_string *var,
                               const s_shell *shell, int only_unset)
{
    (void) param;
    (void) var;
    (void) only_unset;
    (void) shell;
    //TODO
    return NULL;
}

static s_string *expand_error(const char *param, const s_string *var,
                              int only_unset)
{
    (void) param;
    (void) var;
    (void) only_unset;
    //TODO
    return NULL;
}

static s_string *expand_del_prefix(const char *param, const s_string *var)
{
    (void) param;
    (void) var;
    //TODO
    return NULL;
}

static s_string *expand_del_suffix(const char *param, const s_string *var)
{
    (void) param;
    (void) var;
    //TODO
    return NULL;
}

s_string *expand_substs_param(const s_shell *shell, const char *param,
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
        return Func(param, __VA_ARGS__);
#include "expand_params.def"
#undef X
    return NULL;
}

s_string *expand_substs_var(const s_shell *shell, const s_string *word)
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

#include <stdlib.h>
#include <string.h>

#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"

typedef struct expand_params
{
    s_shell *shell;
    const char *word;
    const s_string *varname;
    const s_string *varcont;
    int only_unset;
} s_expand_params;

/*
** Use Alternative Value. If parameter is unset or null, null shall be
** substituted; otherwise, the expansion of word shall be substituted.
*/
static s_string *expand_alternative(s_expand_params *p)
{
    // TODO: expand word
    if (!p->varcont || (!p->varcont->buf[0] && !p->only_unset))
        return string_create_from("");
    else
        return string_create_from(p->word);
}

/*
** Use Default Values. If parameter is unset or null, the expansion of word
** shall be substituted; otherwise, the value of parameter shall be
** substituted.
*/
static s_string *expand_default(s_expand_params *p)
{
    // TODO: expand word
    if (!p->varcont || (!p->varcont->buf[0] && !p->only_unset))
        return string_create_from(p->word);
    else
        return string_duplicate(p->varcont);
    return NULL;
}

/*
** Assign Default Values. If parameter is unset or null, the expansion of word
** shall be assigned to parameter. In all cases, the final value of parameter
** shall be substituted. Only variables, not positional parameters or special
** parameters, can be assigned in this way.
*/
static s_string *expand_assign(s_expand_params *p)
{
    // TODO: expand word
    if (!p->varcont || (!p->varcont->buf[0] && !p->only_unset))
    {
    //    char *name = strdup(p->varname->buf);
    //    env_set(p->shell, p->word, name);
    //TODO
    }
    return string_create_from(env_get(p->shell, p->varname->buf));
}

/*
** Indicate Error if Null or Unset. If parameter is unset or null, the
** expansion of word (or a message indicating it is unset if word is omitted)
** shall be written to standard error and the shell exits with a non-zero exit
** status. Otherwise, the value of parameter shall be substituted. An
** interactive shell need not exit.
*/
static s_string *expand_error(s_expand_params *p)
{
    //TODO
    (void) p;
    return NULL;
}

static s_string *expand_del_prefix(s_expand_params *p)
{
    //TODO
    (void) p;
    return NULL;
}

static s_string *expand_del_suffix(s_expand_params *p)
{
    //TODO
    (void) p;
    return NULL;
}

s_string *expand_substs_param(s_shell *shell, const char *param,
                              const s_string *varname, const s_string *varcont)
{
    s_expand_params p;
    p.only_unset = 1;
    if (param[0] == ':')
    {
        param++;
        p.only_unset = 0;
    }
    p.shell = shell;
    p.word = param + 1;
    p.varname = varname;
    p.varcont = varcont;
#define X(Op, Func, ...)                        \
    if (param[0] == (#Op)[0])                   \
        return Func(&p);
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

    s_string *varcont = string_create_from(env_get(shell, varname->buf));
    if (!word->buf[i])
        return varcont;

    s_string *r = expand_substs_param(shell, word->buf + i, varname, varcont);

    free(varname);
    free(varcont);
    return r;
}

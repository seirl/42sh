#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "env.h"
#include "expand.h"
#include "match.h"
#include "shell.h"
#include "string_utils.h"

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
        char *name = strdup(p->varname->buf);
        char *value = strdup(p->word);
        env_set(p->shell, value, name);
        free(name);
        free(value);
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
    //TODO: expand word
    //TODO: exit(1)
    if (!p->varcont || (!p->varcont->buf[0] && !p->only_unset))
    {
        fprintf(stderr, "%s: %s", p->varname->buf, p->word);
        return NULL;
    }
    else
        return string_create_from(env_get(p->shell, p->varname->buf));
}

static s_string *expand_del_prefix(s_expand_params *p)
{
    //TODO: expand word
    unsigned end_prefix = 0;
    int largest = 0;
    if (*p->word == '#')
    {
        p->word++;
        largest = 1;
    }
    char *n = strdup(p->varcont->buf);
    for (unsigned i = strlen(n); p->varcont->buf[i]; i--)
    {
        n[i] = '\0';
        if (!my_fnmatch(p->word, n))
        {
            end_prefix = i;
            if (largest)
                break;
        }
    }
    s_string *r = string_create_from(p->varcont->buf + end_prefix);
    free(n);
    return r;
}

static s_string *expand_del_suffix(s_expand_params *p)
{
    //TODO: expand word
    unsigned start_suffix = 0;
    int largest = 0;
    if (*p->word == '%')
    {
        p->word++;
        largest = 1;
    }
    for (unsigned i = 0; p->varcont->buf[i]; i++)
        if (!my_fnmatch(p->word, p->varcont->buf + i))
        {
            start_suffix = i;
            if (largest)
                break;
        }
    char *n = strndup(p->varcont->buf, start_suffix);
    s_string *r = string_create_from(n);
    free(n);
    return r;
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

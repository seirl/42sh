#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "env_private.h"
#include "shell_private.h"
#include "hashtbl.h"
#include "env.h"
#include "smalloc.h"
#include "string_utils.h"

extern char **environ;

static void add_cwd(s_shell *shell)
{
    char *curr_dir = scalloc(4096, sizeof (char));
    curr_dir = getcwd(curr_dir, 4096);
    env_set(shell, curr_dir, "PWD");
    sfree(curr_dir);
}

static void add_status(s_shell *shell)
{
    env_set(shell, "0", "?");
    env_chmod(shell, "?", VAR_RDONLY);
}

static void dump_env(s_shell *shell)
{
    s_string *name = string_create(0);
    s_string *value = string_create(0);
    for (char **var = environ; *var != NULL; ++var)
    {
        int i;
        for (i = 0; (*var)[i] != '='; ++i)
            string_putc(name, (*var)[i]);
        for (i += 1; (*var)[i]; ++i)
            string_putc(value, (*var)[i]);
        env_set(shell, value->buf, name->buf);
        string_reset(name);
        string_reset(value);
    }
    string_free(name);
    string_free(value);
}

void env_default_var(s_shell *shell)
{
    dump_env(shell);
    add_cwd(shell);
    add_status(shell);
    env_set(shell, "42sh\\$ ", "PS1");
    env_set(shell, "> ", "PS2");
    env_set(shell, " \t\n", "IFS");
}

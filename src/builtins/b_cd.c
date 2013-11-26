#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "builtins.h"
#include "macros.h"
#include "string.h"
#include "log.h"
#include "shell_private.h"
#include "shell.h"
#include "env.h"

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

static int cd_to_dir(s_shell *shell, char *dir)
{
    char *curr_dir = get_current_dir_name();
    char *new_path = NULL;
    if (chdir(dir))
    {
        fprintf(stderr, "42sh: cd: %s: No such file or directory\n", dir);
        return 1;
    }
    setenv("OLDPWD", curr_dir, 1);
    env_set(shell, curr_dir, "OLDPWD");
    new_path = get_current_dir_name();
    setenv("PWD", new_path, 1);
    env_set(shell, new_path, "PWD");
    return 0;
}

static int cd_to_var(s_shell *shell, char *var)
{
    char *new_dir = env_get(shell, var);
    if (!strcmp(var, "OLDPWD") && new_dir)
        fprintf(stdout, "%s\n", new_dir);

    if (!new_dir)
    {
        if (!(strcmp(var, "OLDPWD")) || !(new_dir = getenv(var)))
        {
            fprintf(stderr, "42sh: cd: %s not set\n", var);
            return 1;
        }
        return cd_to_dir(shell, new_dir);
    }
    else
        return cd_to_dir(shell, new_dir);
}

int builtin_cd(s_shell *shell, int argc, char *argv[])
{
    if (!argc || !argv[1])
        return cd_to_var(shell, "HOME");
    if (!strcmp(argv[1], "-"))
        return cd_to_var(shell, "OLDPWD");
    return cd_to_dir(shell, argv[1]);
}

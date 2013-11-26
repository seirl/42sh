#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "builtins.h"
#include "macros.h"
#include "string.h"
#include "smalloc.h"
#include "log.h"
#include "shell_private.h"
#include "shell.h"
#include "env.h"

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

static char *path_concat(char *dir)
{
    if (dir && (dir[0] == '/'))
        return dir;

    char *path = getenv("HOME");
    /* final char* : "$HOME/DIR\0" */
    int len_path = strlen(path);
    int len_dir = strlen(dir);
    char *res = smalloc(len_path + len_dir + 2);
    int i = 0;
    for (; i < len_path; ++i)
        res[i] = path[i];
    res[len_path] = '/';
    i += 1;
    for (; i < len_dir + 1 + len_path; ++i)
        res[i] = dir[i - len_path - 1];
    res[len_path + len_dir + 1] = '\0';
    return res;
}

static int cd_to_dir(s_shell *shell, char *dir)
{
    char *pwd = NULL;
    if (!strcmp(dir, "."))
    {
        pwd = getenv("PWD");
        setenv("OLDPWD", pwd, 1);
        env_set(shell, pwd, "OLDPWD");
        return 0;
    }

    if (chdir(dir))
    {
        fprintf(stderr, "42sh: cd: %s: No such file or directory\n", dir);
        return 1;
    }
    if ((pwd = getenv("PWD")))
    {
        setenv("OLDPWD", pwd, 1);
        env_set(shell, pwd, "OLDPWD");
        char *complete_path = path_concat(dir);
        setenv("PWD", complete_path, 1);
        env_set(shell, complete_path, "PWD");
    }
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

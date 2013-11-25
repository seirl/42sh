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
    char *pwd = NULL;

    if (chdir(dir))
    {
        LOG(WARN, "42sh: cd: %s: No such file or directory", dir);
        return 1;
    }
    else if ((pwd = getenv("PWD")))
    {
        setenv("OLDPWD", pwd, 1);
        env_set(shell, pwd, "OLDPWD");
    }
    setenv("PWD", dir, 1);
    env_set(shell, pwd, "OLDPWD");
    return 0;
}

static int cd_to_var(s_shell *shell, char *var)
{
    char *new_dir = env_get(shell, var);

    if (!new_dir)
    {
        LOG(WARN, "42sh: cd: %s not set\n", var);
        return 1;
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

#include <stdlib.h>
#include <stdio.h>
#include "builtins.h"
#include "shell.h"
#include "shell_private.h"
#include "macros.h"
#include "env.h"

int builtin_unset(s_shell *shell, int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
        env_unset(shell, argv[i]);
    return 0;
}

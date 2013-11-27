#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"
#include "shell_private.h"
#include "builtins.h"
#include "macros.h"
#include "log.h"

int builtin_exec(s_shell *shell, int argc, char **argv)
{
    if (argc < 2)
    {
        shell_status_set(shell, 0);
        return 0;
    }
    int res = execvp(argv[1], argv + 1);
    if (res == -1)
    {
        LOG(WARN, "42sh: exec: %s: not found\n", argv[1]);
        shell_status_set(shell, 127);
        return 127;
    }
    return 0;
}

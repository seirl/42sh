#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "smalloc.h"
#include "builtins.h"
#include "shell_private.h"
#include "macros.h"
#include "xsyscall.h"
#include "log.h"
#include "shell.h"

void exit_safe(s_shell *shell, char **argv, int err_code)
{
    if (shell->tmp_fd)
    {
        shell->tmp_fd = 0;
        XCLOSE(10);
    }
    sfree(argv);
    exit(err_code);
}

int builtin_exit(s_shell *shell, int argc, char **argv)
{
    long err_code = 0;
    char *endptr;

    if (argc < 2)
        exit_safe(shell, argv, shell->status);
    if (argc > 2)
    {
        LOG(WARN, "42sh: %s: too many arguments\n", argv[0]);
        return 1;
    }
    err_code = strtol(argv[1], &endptr, 10);
    if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
        fprintf(stdout, "exit\n");
    if (endptr && *endptr)
    {
        LOG(WARN, "42sh: exit: %s: numeric argument required\n", argv[1]);
        exit_safe(shell, argv, 255);
        return 1;
    }
    exit_safe(shell, argv, err_code);
    return 1;
}

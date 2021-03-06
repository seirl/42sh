#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "builtins.h"
#include "macros.h"
#include "string.h"
#include "shell_private.h"
#include "shell.h"
#include "env.h"

static int break_error_nb(s_shell *shell)
{
    shell->breaks = shell->loops;
    fprintf(stderr, "break: too many arguments\n");
    return 1;
}

static int break_error_zero(s_shell *shell)
{
    shell->breaks = shell->loops;
    fprintf(stderr, "break: argument is not positive: 0\n");
    return 1;
}

static int break_error_neg(s_shell *shell, long nb)
{
    shell->breaks = shell->loops;
    fprintf(stderr, "break: argument is not positive: %ld\n", nb);
    return 1;
}

int builtin_break(s_shell *shell, int argc, char *argv[])
{
    long n = 0;
    char *endptr = NULL;
    if (argc < 2)
        shell->breaks = 1;
    else if (argc > 2)
        return break_error_nb(shell);
    else
    {
        n = strtol(argv[1], &endptr, 10);
        if ((n == 0) || (endptr && strcmp("", endptr)))
            return break_error_zero(shell);
        else if (n < 0)
            return break_error_neg(shell, n);
        else if (n > shell->loops)
            shell->breaks = shell->loops;
        else
            shell->breaks = n;
    }
    return 0;
}

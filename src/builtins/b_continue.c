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

static int continue_error_nb(void)
{
    fprintf(stderr, "continue: too many arguments\n");
    return 1;
}

static int continue_error_zero(void)
{
    fprintf(stderr, "continue: argument is not positive: 0\n");
    return 1;
}

static int continue_error_neg(long nb)
{
    fprintf(stderr, "continue: argument is not positive: %ld\n", nb);
    return 1;
}

int builtin_continue(s_shell *shell, int argc, char *argv[])
{
    long n = 0;
    char *endptr = NULL;
    if (argc < 2)
        shell->continues = 1;
    else if (argc > 2)
        return continue_error_nb();
    else
    {
        n = strtol(argv[1], &endptr, 10);
        if ((n == 0) || (endptr && strcmp("", endptr)))
            return continue_error_zero();
        else if (n < 0)
            return continue_error_neg(n);
        else
            shell->continues = n;
    }
    return 0;
}

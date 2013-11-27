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

int builtin_break(s_shell *shell, int argc, char *argv[])
{
    long n = 0;
    char *endptr = NULL;

    if (argc < 2)
        shell->breaks = 1;
    else if (argc > 2)
    {
        fprintf(stderr, "break: too many arguments\n");
        return 1;
    }
    else
    {
        n = strtol(argv[1], &endptr, 10);
        if ((n == 0) || (endptr && strcmp("", endptr)))
        {
            fprintf(stderr, "break: argument is not positive: 0\n");
            return 1;
        }
        else if (n < 0)
        {
            fprintf(stderr, "break: argument is not positive: %ld\n", n);
            return 1;
        }
        else
            shell->breaks = n;
    }
    return 0;
}

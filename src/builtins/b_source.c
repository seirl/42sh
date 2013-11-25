#include <stdlib.h>
#include <stdio.h>
#include "builtins.h"
#include "shell_private.h"
#include "macros.h"
#include "shell.h"
#include "log.h"
#include "shell_eval.h"

int builtin_source(s_shell *shell, int argc, char **argv)
{
    FILE *f = NULL;
    if (argc < 2)
    {
        LOG(WARN, "42sh: %s: filename argument requires\n", argv[0]);
        LOG(WARN, "%s: usage: source filename [arguments]\n", argv[0]);
        return 1;
    }
    if (!(f = fopen(argv[1], "r")))
    {
        LOG(WARN, "42sh: source: %s: file not found\n", argv[1]);
        return 1;
    }
    shell_eval_file(shell, f, argv[1]);
    return 0;
}
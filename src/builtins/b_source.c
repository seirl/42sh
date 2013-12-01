#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "builtins.h"
#include "shell_private.h"
#include "macros.h"
#include "shell.h"
#include "log.h"
#include "shell_eval.h"

int builtin_source(s_shell *shell, int argc, char **argv)
{
    FILE *f = NULL;
    if (argc < 2 || ((argc == 2) && (!argv[1] || (!strcmp(argv[1], "")))))
    {
        LOG(WARN, "42sh: %s: filename argument requires\n", argv[0]);
        LOG(WARN, "%s: usage: source filename [arguments]\n", argv[0]);
        shell_status_set(shell, 2);
        return 2;
    }
    if (!(f = fopen(argv[1], "r")))
    {
        LOG(WARN, "42sh: source: %s: file not found\n", argv[1]);
        shell_status_set(shell, 1);
        return 1;
    }
    s_parser *parser = shell->parser;
    shell->parser = NULL;
    int ret = shell_eval_file(shell, f, argv[1]);
    shell->parser = parser;
    return ret;
}

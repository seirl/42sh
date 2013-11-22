#include <stdlib.h>
#include <stdio.h>
#include "options.h"
#include "shell.h"

int main(int argc, char *argv[])
{
    char *cmd;
    char *file;
    s_shell *shell = shell_new();
    int ret = parse_options(shell, argc, argv, &cmd, &file);
    shell_delete(shell);
    if (ret == 2)
        return ret;

    return EXIT_SUCCESS;
}

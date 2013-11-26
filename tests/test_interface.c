#include <stdlib.h>
#include <stdio.h>

#include "options.h"
#include "smalloc.h"
#include "shell.h"

int main(int argc, char *argv[])
{
    char *src;
    s_shell *shell = shell_new();

    int ret = parse_options(shell, argc, argv, &src);

    shell_delete(shell);
    smalloc_clean();

    if (ret & E_ERROR)
        return ret;
    return EXIT_SUCCESS;
}

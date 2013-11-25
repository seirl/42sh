#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#ifndef _POSIX_SOURCE
# define _POSIX_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "input_interactive.h"

#include "input_private.h"
#include "input_string.h"
#include "input_file.h"
#include "shell.h"
#include "macros.h"

s_input *input_create(struct shell *shell, char *src, int mode)
{
    s_string *input_str;
    FILE *f;
    if (mode & SRC_STR)
    {
        input_str = string_create_from(src);
        return input_string_create(input_str, "<cmd>");
    }
    if (mode & SRC_FILE)
    {
        if ((f = fopen(src, "r")) == NULL)
            RET_WITH(NULL, PROGNAME": %s: No such file or directory\n", src);
        return input_file_create(f, src);
    }
    if (!isatty(STDIN_FILENO))
    {
        f = fdopen(STDIN_FILENO, "r");
        return input_file_create(f, "<stdin>");
    }
    return input_interactive_create(shell);
}

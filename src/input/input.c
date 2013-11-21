#define GNU_SOURCE
#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "input_interactive.h"

#include "input_private.h"
#include "input_string.h"
#include "input_file.h"
#include "shell.h"

s_input *input_create(s_shell *shell,
                      char **cmd,
                      char *file,
                      e_shell_repeat repeat)
{
    s_string *input_str;
    FILE *f;
    if (*cmd && !repeat)
    {
        input_str = string_create_from(*cmd);
        return input_string_create(input_str, "<cmd>");
    }
    if (file)
    {
        if ((f = fopen(file, "r")) == NULL)
            RET_WITH(NULL, PROGNAME": %s: No such file or directory\n", file);
        return input_file_create(f, file);
    }
    if (!(repeat = isatty(STDIN_FILENO)))
    {
        f = fdopen(STDIN_FILENO, "r");
        return input_file_create(f, "<stdin>");
    }
    return input_interactive_create(shell);
}

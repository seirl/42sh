#include <stdio.h>
#include "string_utils.h"
#include "readline.h"
#include "env.h"

int main(void)
{
    s_string *line;
    s_shell *shell = shell_new();
    while ((line = readline(shell, "test$ ")))
    {
        printf("Read: [%s]\n", line->buf);
        string_free(line);
    }
    shell_delete(shell);
    return 0;
}

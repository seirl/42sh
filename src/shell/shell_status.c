#include <stdlib.h>

#include "shell_private.h"
#include "env.h"

int shell_status(s_shell *shell)
{
    return shell->status;
}

void shell_status_set(s_shell *shell, int status)
{
    shell->status = status;

    char *status_char = NULL;
    asprintf(&status_char, "%d", status);
    env_force_set(shell, status_char, "?");
    free(status_char);
}

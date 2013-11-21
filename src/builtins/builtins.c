#include <string.h>

#include "builtins.h"

#include "shell_private.h"

f_handler builtins_find(s_shell *shell, const char *name)
{
    if (shell->builtins || !shell->builtins_count)
        return NULL;
    for (size_t i = 0; i < shell->builtins_count; ++i)
        if (!strcmp(shell->builtins[i].name, name))
            return shell->builtins[i].callback;
    return NULL;
}

#include <string.h>

#include "builtins.h"
#include "shell_private.h"
#include "smalloc.h"

void builtins_create(s_shell *shell)
{
    shell->builtins_count = 1 +
#define X(Name) + 1
#include "builtins.def"
#undef X
    ;
    shell->builtins = smalloc(sizeof (s_builtins) * shell->builtins_count);
    int i = 0;
#define X(Name)                                      \
    shell->builtins[i].name = #Name;                 \
    shell->builtins[i].callback = builtin_##Name;    \
    ++i;
#include "builtins.def"
#undef X
    shell->builtins[i].name = ".";
    shell->builtins[i].callback = builtin_source;
}

f_handler builtins_find(s_shell *shell, const char *name)
{
    if (shell->builtins == NULL || shell->builtins_count == 0)
        return NULL;
    for (size_t i = 0; i < shell->builtins_count; ++i)
        if (!strcmp(shell->builtins[i].name, name))
            return shell->builtins[i].callback;
    return NULL;
}

void builtins_free(s_shell *shell)
{
    sfree(shell->builtins);
}

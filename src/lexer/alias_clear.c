#include <stdio.h>
#include "alias.h"
#include "hashtbl.h"
#include "shell_private.h"

void alias_clear(s_shell *shell)
{
    hashtbl_free(shell->aliases);
    alias_create(shell);
}

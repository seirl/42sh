#include "shell_private.h"
#include "env_private.h"
#include "functions_private.h"
#include "shopt.h"
#include "smalloc.h"
#include "xsyscall.h"
#include "history.h"
#include "autocompletion.h"

void shell_delete_duplicate(s_shell *shell)
{
    if (shell->tmp_fd)
        XCLOSE(10);
    history_close(shell);
    rehash_free();
    env_free(shell);
    builtins_free(shell);

    /** Not freed (inherited from parent)
    ** - alias_free(shell);
    ** - functions_free(shell);
    */

    sfree(shell);
}

s_shell *shell_duplicate(s_shell *shell)
{
    s_shell *ret = scalloc(1, sizeof (s_shell));

    ret->env = env_duplicate(shell);
    ret->functions = shell->functions;
    ret->aliases = shell->aliases;

    shopt_create(ret);
    builtins_create(ret);

    ret->curr_argv = NULL;
    ret->parser = NULL;
    ret->state = SHELL_STOP_ON_ERROR;
    ret->status = 0;
    ret->breaks = 0;
    ret->arg_count = 0;
    ret->loops = 0;
    ret->tmp_fd = 0;

    return ret;
}

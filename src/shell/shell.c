#include <assert.h>

#include "shell_private.h"

#include "env_private.h"
#include "functions_private.h"
#include "shopt.h"
#include "env.h"
#include "smalloc.h"
#include "xsyscall.h"
#include "history.h"
#include "autocompletion.h"

s_shell *shell_new(void)
{
    s_shell *shell = scalloc(1, sizeof (s_shell));

    env_create(shell);
    functions_init(shell);
    shopt_create(shell);
    builtins_create(shell);
    alias_create(shell);

    shell->curr_argv = NULL;
    shell->parser = NULL;
    shell->state = SHELL_STOP_ON_ERROR;
    shell->status = 0;
    shell->breaks = 0;
    shell->arg_count = 0;
    shell->loops = 0;
    shell->tmp_fd = 0;

    return shell;
}

void shell_setup(s_shell *shell, s_parser *parser)
{
    shell->parser = parser;
    rehash(shell);
}

void shell_delete(s_shell *shell)
{
    if (shell->tmp_fd)
        XCLOSE(10);
    history_close(shell);
    rehash_free();
    env_free(shell);
    functions_free(shell);
    builtins_free(shell);
    alias_free(shell);

    sfree(shell);
}

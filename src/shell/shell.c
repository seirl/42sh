#include <assert.h>

#include "shell_private.h"

#include "env_private.h"
#include "functions_private.h"
#include "shopt.h"
#include "smalloc.h"

s_shell *shell_new(void)
{
    s_shell *shell = scalloc(1, sizeof (s_shell));

    env_create(shell);
    functions_init(shell);
    shopt_create(shell);
    builtins_create(shell);

    shell->curr_argv = NULL;
    shell->parser = NULL;
    shell->state = SHELL_REPEAT;
    shell->status = 0;

    return shell;
}

void shell_setup(s_shell *shell, s_parser *parser)
{
    assert(!shell->parser && "There is already a parser setup.");
    shell->parser = parser;
}

void shell_delete(s_shell *shell)
{
    env_free(shell);
    functions_free(shell);
    builtins_free(shell);

    sfree(shell);
}

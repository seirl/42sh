#include <assert.h>

#include "shell_private.h"

#include "env_private.h"
#include "functions_private.h"
#include "smalloc.h"

s_shell *shell_new(void)
{
    s_shell *shell = smalloc(sizeof (s_shell));

    env_create(shell);
    functions_init(shell);

    shell->builtins = NULL;
    shell->builtins_count = 0;
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
    // TODO: free shell->builtins;

    sfree(shell);
}

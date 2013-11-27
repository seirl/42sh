#include "exec.h"
#include "shell_private.h"

void exec_and_node(s_shell *shell, s_ast_and_or *node, int force)
{
    if (!node)
        return;
    if (force != 1)
    {
        if (node->pipeline)
            exec_pipe_node(shell, node->pipeline);
        if (force == 0)
            shell->status = 0;
        if (!shell->status && node->next)
            exec_andor_node(shell, node->next, -1);
        else if (shell->status && node->next)
            exec_andor_node(shell, node->next, 1);
    }
    else
        if (node->next)
            exec_andor_node(shell, node->next, -1);
}

void exec_or_node(s_shell *shell, s_ast_and_or *node, int force)
{
    if (!node)
    {
        fprintf(stderr, "Wrong '||' command.\n");
        return;
    }
    if (force != 0)
        if (node->pipeline)
            exec_pipe_node(shell, node->pipeline);
    if (force == 0)
    {
        if (node->next)
            exec_andor_node(shell, node->next, 0);
    }
    else if (node->next)
    {
        if (!shell->status)
            exec_andor_node(shell, node->next, 0);
        else
            exec_andor_node(shell, node->next, -1);
    }
}

/* force: -1 means that you won’t force error code value */
/*         0 means that you do force it to 0             */
/*         1 means that you do force it to 1             */
void exec_andor_node(s_shell *shell, s_ast_and_or *node, int force)
{
    if (!node)
    {
        fprintf(stderr, "Wrong '&&' or '||' command.\n");
        return;
    }
    if (node->and_or == AST_CMD_AND)
        exec_and_node(shell, node, force);
    else
        exec_or_node(shell, node, force);
    node = node->next;
}

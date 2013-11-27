#include "exec.h"
#include "shell_private.h"

void exec_and_node(s_shell *shell, s_ast_and_or *node)
{
    if (!node)
    {
        fprintf(stderr, "Wrong '&&' command.\n");
        return;
    }
    if (node->pipeline)
        exec_pipe_node(shell, node->pipeline);
    if (!shell->status && node->next)
        exec_andor_node(shell, node->next);
}

void exec_or_node(s_shell *shell, s_ast_and_or *node)
{
    if (!node)
    {
        fprintf(stderr, "Wrong '||' command.\n");
        return;
    }
    if (node->pipeline)
        exec_pipe_node(shell, node->pipeline);
    if (node->next)
    {
        if (!shell->status)
        {
            exec_andor_node(shell, node->next);
            shell->status = 0;
        }
        else
            exec_andor_node(shell, node->next);
    }
}

void exec_andor_node(s_shell *shell, s_ast_and_or *node)
{
    if (!node)
    {
        fprintf(stderr, "Wrong '&&' or '||' command.\n");
        return;
    }
    if (node->and_or == AST_CMD_AND)
        exec_and_node(shell, node);
    else
        exec_or_node(shell, node);
    node = node->next;
}

#include "exec.h"

void exec_and_node(s_ast_and_or *node)
{
    if (!node)
    {
        fprintf(stderr, "Wrong '&&' command.\n");
        return;
    }
    if (node->pipeline)
        exec_pipe_node(node->pipeline);
    if (shell.status == 0 && node->next)
        exec_andor_node(node->next);
}

void exec_or_node(s_ast_and_or *node)
{
    if (!node)
    {
        fprintf(stderr, "Wrong '||' command.\n");
        return;
    }
    if (node->pipeline)
        exec_pipe_node(node->pipeline);
    if (node->next)
        exec_andor_node(node->next);
}

void exec_andor_node(s_ast_and_or *node)
{
    if (!node)
    {
        fprintf(stderr, "Wrong '&&' or '||' command.\n");
        return;
    }
    if (node->and_or == AST_CMD_AND)
        exec_and_node(node);
    else
        exec_or_node(node);
    node = node->next;
}

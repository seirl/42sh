#include "exec.h"

int pipe_cmd_count(s_ast_pipeline *node)
{
    int count = 0;
    while (node)
    {
        count += 1;
        node = node->next;
    }
    return count;
}

s_ast_cmd **pipe_cmd_array(s_ast_pipeline *node, int len)
{
    s_ast_cmd **pipe_cmds = smalloc(sizeof (s_ast_pipeline *) * len);

    for (int i = 0; i < len; ++i)
    {
        pipe_cmds[i] = node->cmd;
        node = node->next;
    }
    return pipe_cmds;
}

void exec_pipe_setio(int pipe[2], int io)
{
    if (io)
    {
        close(pipe[1]);
        dup2(pipe[0], 0);
        close(pipe[0]);
    }
    else
    {
        close(pipe[0]);
        dup2(pipe[1], 1);
        close(pipe[1]);
    }
}

#include "exec.h"

void exec_ast_list(s_shell *shell, s_ast_list *list)
{
    s_ast_list *tmp = list;
    while (tmp)
    {
        exec_andor_node(shell, tmp->and_or, -1);
        tmp = tmp->next;
    }
}

void exec_ast_input(s_shell *shell, s_ast_input *ast)
{
    exec_ast_list(shell, ast->list);
}

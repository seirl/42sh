#include "exec.h"

void exec_ast_list(s_shell *shell, s_ast_list *list)
{
    s_ast_list *tmp = list;
    while (tmp)
    {
        if (shell->continues == 1 || (shell->continues && shell->loops == 1))
        {
            tmp = tmp->next;
            continue;
        }
        else
        {
            if (shell->continues > 1 || shell->breaks)
                return;
            exec_andor_node(shell, tmp->and_or);
            tmp = tmp->next;
        }
    }
    if (shell->continues == 1)
        shell->continues -= 1;
}

void exec_ast_input(s_shell *shell, s_ast_input *ast)
{
    exec_ast_list(shell, ast->list);
}

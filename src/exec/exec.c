#include "exec.h"

s_shell g_shell;

void shell_free(void)
{
    vars_free();
    funcs_free();
    if (g_shell.curr_argv)
        sfree(g_shell.curr_argv);
}

void exec_ast_list(s_ast_list *list)
{
    while (list)
    {
        exec_andor_node(list->and_or);
        list = list->next;
    }
}

void exec_ast_input(s_ast_input *ast)
{
    exec_ast_list(ast->list);
}

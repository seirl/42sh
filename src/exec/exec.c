#include "exec.h"

s_shell shell;

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

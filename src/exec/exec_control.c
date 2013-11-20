#include "exec.h"

void exec_else(s_ast_else *else_cmd)
{
    exec_ast_list(else_cmd->elif_predicate);
    if (!g_shell.status)
        exec_ast_list(else_cmd->elif_cmds);
    else
        exec_ast_list(else_cmd->else_cmds);
}

void exec_if(s_ast_if *if_cmd)
{
    exec_ast_list(if_cmd->predicate);
    if (!g_shell.status)
        exec_ast_list(if_cmd->then_cmds);
    /* FIXME: elif */
    else
        if (if_cmd->else_clause)
            exec_else(if_cmd->else_clause);
}

void exec_while(s_ast_while *while_cmd)
{
    for (exec_ast_list(while_cmd->predicate);
         !g_shell.status;
         exec_ast_list(while_cmd->predicate))
    {
        exec_ast_list(while_cmd->cmds);
    }
}

void exec_until(s_ast_until *until_cmd)
{
    for (exec_ast_list(until_cmd->predicate);
         g_shell.status;
         exec_ast_list(until_cmd->predicate))
    {
        exec_ast_list(until_cmd->cmds);
    }
}

void exec_for(s_ast_for *for_cmd)
{
    s_string *value = NULL;
    s_ast_word_list *values = for_cmd->values;
    while (values && (value = expand_compound(values->word)))
    {
        vars_set(expand_word(for_cmd->identifier), value);
        value = NULL;
        exec_ast_list(for_cmd->cmd_list);
        values = values->next;
    }
}

#include "exec.h"
#include "shell_private.h"
#include "env.h"

void exec_else(s_shell *shell, s_ast_else *else_cmd)
{
    exec_ast_list(shell, else_cmd->elif_predicate);
    if (!shell->status)
        exec_ast_list(shell, else_cmd->elif_cmds);
    else
        exec_ast_list(shell, else_cmd->else_cmds);
}

void exec_if(s_shell *shell, s_ast_if *if_cmd)
{
    exec_ast_list(shell, if_cmd->predicate);
    if (!shell->status)
        exec_ast_list(shell, if_cmd->then_cmds);
    /* FIXME: elif */
    else
        if (if_cmd->else_clause)
            exec_else(shell, if_cmd->else_clause);
}

void exec_while(s_shell *shell, s_ast_while *while_cmd)
{
    for (exec_ast_list(shell, while_cmd->predicate); !shell->status;
                                 exec_ast_list(shell, while_cmd->predicate))
        exec_ast_list(shell, while_cmd->cmds);
}

void exec_until(s_shell *shell, s_ast_until *until_cmd)
{
    for (exec_ast_list(shell, until_cmd->predicate); shell->status;
                                 exec_ast_list(shell, until_cmd->predicate))
        exec_ast_list(shell, until_cmd->cmds);
}

void exec_for(s_shell *shell, s_ast_for *for_cmd)
{
    s_ast_word_list *values = for_cmd->values;
    while (values && expand_compound(values->word))
    {
        s_string *id = string_duplicate(expand_word(for_cmd->identifier));
        s_string *value = string_duplicate(expand_compound(values->word));
        exec_ast_list(shell, for_cmd->cmd_list);
        env_set(shell, string_release(id), string_release(value));
        values = values->next;
    }
}

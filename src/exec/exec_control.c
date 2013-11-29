#include "exec.h"
#include "shell_private.h"
#include "env.h"
#include "expand.h"

static void exec_else(s_shell *shell, s_ast_else *else_clause)
{
    if (shell->breaks)
    {
        --shell->breaks;
        return;
    }
    if (else_clause->elif_predicate)
    {
        exec_ast_list(shell, else_clause->elif_predicate);
        if (!shell->status)
            exec_ast_list(shell, else_clause->elif_cmds);
        else
            if (!else_clause->else_cmds)
                exec_else(shell, else_clause->next_else);
    }
    else
        exec_ast_list(shell, else_clause->else_cmds);
}

void exec_if(s_shell *shell, s_ast_if *if_cmd)
{
    exec_ast_list(shell, if_cmd->predicate);
    if (!shell->status)
        exec_ast_list(shell, if_cmd->then_cmds);
    else
        if (if_cmd->else_clause)
            exec_else(shell, if_cmd->else_clause);
}

void exec_while(s_shell *shell, s_ast_while *while_cmd)
{
    for (exec_ast_list(shell, while_cmd->predicate); !shell->status;
                                 exec_ast_list(shell, while_cmd->predicate))
    {
        if (shell->breaks)
        {
            --shell->breaks;
            return;
        }
        exec_ast_list(shell, while_cmd->cmds);
        if (shell->breaks)
        {
            --shell->breaks;
            return;
        }
    }
    shell_status_set(shell, 0);
}

void exec_until(s_shell *shell, s_ast_until *until_cmd)
{
    for (exec_ast_list(shell, until_cmd->predicate); shell->status;
                                 exec_ast_list(shell, until_cmd->predicate))
    {
        if (shell->breaks)
        {
            --shell->breaks;
            return;
        }
        exec_ast_list(shell, until_cmd->cmds);
        if (shell->breaks)
        {
            --shell->breaks;
            return;
        }
    }
}

void exec_for(s_shell *shell, s_ast_for *for_cmd)
{
    expand_wordlist(shell, for_cmd->values);
    s_ast_word_list *values = for_cmd->values;
    while (values && expand_compound(shell, values->word))
    {
        if (shell->breaks && shell->breaks--)
            return;
        s_string *id = string_duplicate(expand_word(for_cmd->identifier));
        s_string *value = string_duplicate(expand_compound(shell,
                                                          values->word));
        env_set(shell, string_release(value), string_release(id));
        exec_ast_list(shell, for_cmd->cmd_list);
        if (shell->breaks && shell->breaks--)
            return;
        values = values->next;
    }
}

static int exec_case_match(s_shell *shell,
                           s_string *id,
                           s_ast_word_list *match,
                           s_ast_list *cmd)
{
    while (match)
    {
        s_string *val = expand_compound(shell, match->word);
        if (!my_fnmatch(val->buf, id->buf))
        {
            exec_ast_list(shell, cmd);
            return 0;
        }
        match = match->next;
    }
    return 1;
}

static void exec_clauses(s_shell *shell,
                         s_string *id,
                         s_ast_case_item *clauses)
{
    while (clauses)
    {
        if (!exec_case_match(shell,
                             id,
                             clauses->match,
                             clauses->cmd_list))
            return;
        clauses = clauses->next;
    }
    shell_status_set(shell, 0);
}

void exec_case(s_shell *shell, s_ast_case *case_cmd)
{
    assert(case_cmd && case_cmd->word && case_cmd->clauses);
    s_string *id = expand_compound(shell, case_cmd->word);
    exec_clauses(shell, id, case_cmd->clauses);
}

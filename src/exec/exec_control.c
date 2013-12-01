#include "exec.h"
#include "shell_private.h"
#include "env.h"
#include "expand.h"

static void exec_else(s_shell *shell, s_ast_else *else_clause)
{
    if (!else_clause)
        return;
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
    shell->loops += 1;
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
    shell->loops -= 1;
    shell_status_set(shell, 0);
}

void exec_until(s_shell *shell, s_ast_until *until_cmd)
{
    shell->loops += 1;
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
    shell->loops -= 1;
}

void exec_for(s_shell *shell, s_ast_for *for_cmd)
{
    expand_wordlist(shell, for_cmd->values);
    s_ast_word_list *values = for_cmd->values;
    shell->loops += 1;
    while (values)
    {
        expand_wordlist(shell, values);
        if (shell->breaks && shell->breaks--)
            return;
        s_string *id = expand_word(for_cmd->identifier);
        s_string *value = expand_compound(shell, values->word);
        env_set(shell, value->buf, id->buf);
        string_free(value);
        if (shell->breaks && shell->breaks--)
            return;
        else
            exec_ast_list(shell, for_cmd->cmd_list);

        values = values->next;
    }
    shell->loops -= 1;
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
        string_free(val);
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
    string_free(id);
}

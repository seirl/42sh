#include "exec.h"
#include "functions.h"

static void exec_subshell_cmd(s_shell *shell, s_ast_shell_cmd *shell_cmd)
{
    if (!shell_cmd)
    {
        fprintf(stderr, "Invalid SUBSHELL command.\n");
        return;
    }
    exec_shell_cmd(shell, shell_cmd);
    fprintf(stderr, "SUBSHELL commands not implemented yet.\n");
    return;
}

void exec_shell_cmd(s_shell *shell, s_ast_shell_cmd *shell_cmd)
{
    if (shell_cmd->cmd_list)
    {
        if (shell_cmd->subshell == 1)
            exec_subshell_cmd(shell, shell_cmd);
        else
            exec_ast_list(shell, shell_cmd->cmd_list);
    }
    else
    {
        if (shell_cmd->ctrl_structure == AST_IF)
            exec_if(shell, shell_cmd->ctrl.ast_if);
        else if (shell_cmd->ctrl_structure == AST_FOR)
            exec_for(shell, shell_cmd->ctrl.ast_for);
        else if (shell_cmd->ctrl_structure == AST_WHILE)
            exec_while(shell, shell_cmd->ctrl.ast_while);
        else if (shell_cmd->ctrl_structure == AST_UNTIL)
            exec_until(shell, shell_cmd->ctrl.ast_until);
        else if (shell_cmd->ctrl_structure == AST_CASE)
            exec_case(shell, shell_cmd->ctrl.ast_case);
    }
}

void exec_shell_cmd_node(s_shell *shell, s_ast_shell_cmd *shell_cmd,
                         s_ast_redirection_list *redir)
{
    s_redir_context *cont = exec_redirection(shell, redir);
    exec_shell_cmd(shell, shell_cmd);
    restore_redir_context(cont);
}

void exec_func_dec(s_shell *shell, s_ast_funcdec *funcdec)
{
    functions_set(shell, funcdec->name->buf, funcdec->shell_cmd);
    funcdec->shell_cmd = NULL;
}

void exec_funcdec_node(s_shell *shell, s_ast_funcdec *funcdec,
                       s_ast_redirection_list *redir)
{
    s_redir_context *cont = exec_redirection(shell, redir);
    exec_func_dec(shell, funcdec);
    restore_redir_context(cont);
}

void exec_cmd_node(s_shell *shell, s_ast_cmd *node)
{
    if (node->simple_cmd)
        exec_simple_cmd(shell, node->simple_cmd);
    else if (node->shell_cmd)
        exec_shell_cmd_node(shell, node->shell_cmd, node->redirections);
    else if (node->func_dec)
        exec_funcdec_node(shell, node->func_dec, node->redirections);
    else
        fprintf(stderr, "unkown command type.\n");
}

#include "exec.h"

static void exec_subshell_cmd(s_ast_shell_cmd *shell_cmd)
{
    if (!shell_cmd)
    {
        fprintf(stderr, "Invalid SUBSHELL command.\n");
        return;
    }
    exec_shell_cmd(shell_cmd);
    fprintf(stderr, "SUBSHELL commands not implemented yet.\n");
    return;
}

static void exec_case(s_ast_case *node)
{
    if (!node)
    {
        fprintf(stderr, "Invalid CASE command.\n");
        return;
    }
    fprintf(stderr, "CASE commands not implemented yet.\n");
    return;
}

void exec_shell_cmd(s_ast_shell_cmd *shell_cmd)
{
    if (shell_cmd->cmd_list)
    {
        if (shell_cmd->subshell == 1)
            exec_subshell_cmd(shell_cmd);
        else
            exec_ast_list(shell_cmd->cmd_list);
    }
    else
    {
        if (shell_cmd->ctrl_structure == AST_IF)
            exec_if(shell_cmd->ctrl.ast_if);
        else if (shell_cmd->ctrl_structure == AST_FOR)
            exec_for(shell_cmd->ctrl.ast_for);
        else if (shell_cmd->ctrl_structure == AST_WHILE)
            exec_while(shell_cmd->ctrl.ast_while);
        else if (shell_cmd->ctrl_structure == AST_UNTIL)
            exec_until(shell_cmd->ctrl.ast_until);
        else if (shell_cmd->ctrl_structure == AST_CASE)
            exec_case(shell_cmd->ctrl.ast_case);
    }
}

void exec_shell_cmd_node(s_ast_shell_cmd *shell_cmd,
                         s_ast_redirection_list *redir)
{
    exec_redirection(redir);
    exec_shell_cmd(shell_cmd);
}

void exec_func_dec(s_ast_funcdec *funcdec)
{
    funcs_set(funcdec->name->buf, funcdec->shell_cmd);
}

void exec_funcdec_node(s_ast_funcdec *funcdec,
                       s_ast_redirection_list *redir)
{
    exec_redirection(redir);
    exec_func_dec(funcdec);
}

void exec_cmd_node(s_ast_cmd *node)
{
    if (node->simple_cmd)
        exec_simple_cmd(node->simple_cmd);
    else if (node->shell_cmd)
        exec_shell_cmd_node(node->shell_cmd, node->redirections);
    else if (node->func_dec)
        exec_funcdec_node(node->func_dec, node->redirections);
    else
        fprintf(stderr, "unkown command type.\n");
}

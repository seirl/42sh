#include "ast.h"
#include "smalloc.h"

s_ast_cmd *ast_cmd_new(void)
{
    s_ast_cmd *cmd = smalloc(sizeof (s_ast_cmd));

    cmd->simple_cmd = NULL;
    cmd->redirections = NULL;
    cmd->func_dec = NULL;
    cmd->shell_cmd = NULL;

    return cmd;
}

void ast_cmd_delete(s_ast_cmd *cmd)
{
    if (!cmd)
        return;

    ast_simple_cmd_delete(cmd->simple_cmd);
    ast_func_dec_delete(cmd->func_dec);
    ast_shell_cmd_delete(cmd->shell_cmd);
    ast_redirection_list_delete(cmd->redirections);
    sfree(cmd);
}

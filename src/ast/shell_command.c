#include "ast.h"
#include "smalloc.h"

s_ast_shell_cmd *ast_shell_cmd_new(void)
{
    s_ast_shell_cmd *shell_cmd = smalloc(sizeof (s_ast_shell_cmd));

    shell_cmd->cmd_list = NULL;
    shell_cmd->subshell = 0;
    shell_cmd->ctrl.ast_if = NULL;
    shell_cmd->ctrl_structure = 0;

    return shell_cmd;
}

void ast_shell_cmd_delete(s_ast_shell_cmd *cmd)
{
    if (!cmd)
        return;

    ast_list_delete(cmd->cmd_list);
    switch (cmd->ctrl_structure)
    {
    case AST_FOR:
        ast_for_delete(cmd->ctrl.ast_for);
        break;
    case AST_WHILE:
        ast_while_delete(cmd->ctrl.ast_while);
        break;
    case AST_UNTIL:
        ast_until_delete(cmd->ctrl.ast_until);
        break;
    case AST_CASE:
        ast_case_delete(cmd->ctrl.ast_case);
        break;
    case AST_IF:
        ast_if_delete(cmd->ctrl.ast_if);
        break;
    }

    sfree(cmd);
}

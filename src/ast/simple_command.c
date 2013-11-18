#include "ast.h"
#include "smalloc.h"

s_ast_simple_cmd *ast_simple_cmd_new(void)
{
    s_ast_simple_cmd *cmd = smalloc(sizeof (s_ast_simple_cmd));

    cmd->prefixes = NULL;
    cmd->elements = NULL;

    return cmd;
}

void ast_simple_cmd_delete(s_ast_simple_cmd *cmd)
{
    if (!cmd)
        return;

    ast_prefix_delete(cmd->prefixes);
    ast_element_delete(cmd->elements);

    sfree(cmd);
}

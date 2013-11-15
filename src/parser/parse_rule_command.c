#include "parser_private.h"
#include "smalloc.h"

s_ast_cmd *cmd_new(void)
{
    s_ast_cmd *cmd = smalloc(sizeof (s_ast_cmd));

    cmd->simple_cmd = NULL;
    cmd->redirections = NULL;
    cmd->func_dec = NULL;
    cmd->shell_cmd = NULL;

    return cmd;
}

s_ast_cmd *parse_rule_command(s_parser *parser)
{
    s_ast_simple_cmd *simple_cmd;
    s_ast_shell_cmd *shell_cmd;
    s_ast_funcdec *funcdec;
    s_ast_cmd *cmd = cmd_new();

    if ((simple_cmd = parse_rule_simple_command(parser)))
        cmd->simple_cmd = simple_cmd;
    else if ((shell_cmd = parse_rule_shell_command(parser)))
        cmd->shell_cmd = shell_cmd;
    else if ((funcdec = parse_rule_funcdec(parser)))
        cmd->func_dec = funcdec;
    else
    {
        sfree(cmd);
        return NULL;
    }

    return cmd;
}

#include "parser_private.h"

s_ast_cmd *parse_rule_command(s_parser *parser)
{
    s_ast_simple_cmd *simple_cmd;
    s_ast_shell_cmd *shell_cmd;
    s_ast_funcdec *funcdec;
    s_ast_cmd *cmd = ast_cmd_new();

    if ((shell_cmd = parse_rule_shell_command(parser)))
        cmd->shell_cmd = shell_cmd;
    else if ((funcdec = parse_rule_funcdec(parser)))
        cmd->func_dec = funcdec;
    else if ((simple_cmd = parse_rule_simple_command(parser)))
        cmd->simple_cmd = simple_cmd;
    else
    {
        ast_cmd_delete(cmd);
        return NULL;
    }

    cmd->redirections = parse_rule_redirection(parser);

    return cmd;
}

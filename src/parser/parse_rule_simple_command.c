#include "parser_private.h"
#include "smalloc.h"

static s_ast_simple_cmd *simple_cmd_new(void)
{
    s_ast_simple_cmd *cmd = smalloc(sizeof (s_ast_simple_cmd));

    cmd->prefixes = NULL;
    cmd->elements = NULL;

    return cmd;
}

s_ast_simple_cmd *parse_rule_simple_command(s_parser *parser)
{
    s_ast_simple_cmd *cmd = simple_cmd_new();

    cmd->prefixes = parse_rule_prefix(parser);
    cmd->elements = parse_rule_elements(parser);

    return cmd;
}

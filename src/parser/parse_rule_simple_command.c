#include "parser_private.h"

s_ast_simple_cmd *parse_rule_simple_command(s_parser *parser)
{
    s_ast_prefix *prefixes;
    s_ast_element *elements;

    prefixes = parse_rule_prefix(parser);
    elements = parse_rule_elements(parser);

    if (!prefixes && !elements)
        return NULL;

    s_ast_simple_cmd *cmd = ast_simple_cmd_new();

    cmd->prefixes = prefixes;
    cmd->elements = elements;

    return cmd;
}

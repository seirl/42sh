#include "parser_private.h"

// TODO
s_ast_prefix *parse_rule_prefix(s_parser *parser)
{
    s_ast_redirection_list *redirection;
    s_ast_assignment *assignment;

    redirection = parse_rule_redirection(parser);
    assignment = parse_assignment(parser);

    if (!redirection && !assignment)
        return NULL;

    s_ast_prefix *prefix = ast_prefix_new();
    prefix->redirection = redirection;
    prefix->assignment = assignment;

    return prefix;
}

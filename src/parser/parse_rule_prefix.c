#include "parser_private.h"
#include "smalloc.h"

static s_ast_prefix *prefix_new(void)
{
    s_ast_prefix *prefix = smalloc(sizeof (s_ast_prefix));

    prefix->assignment = NULL;
    prefix->redirection = NULL;
    prefix->next = NULL;

    return prefix;
}

s_ast_prefix *parse_rule_prefix(s_parser *parser) // TODO
{
    s_ast_redirection *redirection;
    s_ast_assignment *assignment;

    redirection = parse_rule_redirection(parser);
    assignment = parse_assignment(parser);

    if (!redirection && !assignment)
        return NULL;

    s_ast_prefix *prefix = prefix_new();
    prefix->redirection = redirection;
    prefix->assignment = assignment;

    return NULL;
}

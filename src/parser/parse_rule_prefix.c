#include "parser_private.h"
#include "smalloc.h"

static s_ast_prefix *prefix_new(void)
{
    s_ast_prefix *prefix = smalloc(sizeof (s_ast_prefix));

    prefix->next = NULL;
    prefix->redirection = NULL;

    return prefix;
}

s_ast_prefix *parse_rule_prefix(s_parser *parser) // TODO
{
    (void)parser;
    (void)prefix_new;
    return NULL;
}

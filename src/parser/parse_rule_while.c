#include "parser_private.h"
#include "parser_macros.h"

s_ast_while *parse_rule_while(s_parser *parser)
{
    s_ast_list *predicate;
    s_ast_list *list;

    if (!(predicate = parse_rule_compound_list(parser, 1)))
        return NULL;
    if (!(list = parse_rule_do_group(parser)))
    {
        ast_list_delete(list);
        return NULL;
    }

    s_ast_while *mywhile = ast_while_new();

    mywhile->predicate = predicate;
    mywhile->cmds = list;

    return mywhile;
}

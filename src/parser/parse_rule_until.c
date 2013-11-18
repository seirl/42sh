#include "parser_private.h"
#include "parser_macros.h"

s_ast_until *parse_rule_until(s_parser *parser)
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

    s_ast_until *myuntil = ast_until_new();

    myuntil->predicate = predicate;
    myuntil->cmds = list;

    return myuntil;
}

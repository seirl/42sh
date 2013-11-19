#include "ast.h"
#include "smalloc.h"

s_ast_until *ast_until_new(void)
{
    s_ast_until *myuntil = smalloc(sizeof (s_ast_until));

    myuntil->cmds = NULL;
    myuntil->predicate = NULL;

    return myuntil;
}

void ast_until_delete(s_ast_until *until)
{
    if (!until)
        return;

    ast_list_delete(until->predicate);
    ast_list_delete(until->cmds);

    sfree(until);
}

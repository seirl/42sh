#include "ast.h"
#include "smalloc.h"

s_ast_while *ast_while_new(void)
{
    s_ast_while *mywhile = smalloc(sizeof (s_ast_while));

    mywhile->cmds = NULL;
    mywhile->predicate = NULL;

    return mywhile;
}

void ast_while_delete(s_ast_while *whilee)
{
    if (!whilee)
        return;

    ast_list_delete(whilee->cmds);
    ast_list_delete(whilee->predicate);

    sfree(whilee);
}

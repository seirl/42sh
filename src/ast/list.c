#include "ast.h"
#include "smalloc.h"

s_ast_list *ast_list_new(void)
{
    s_ast_list *list = smalloc(sizeof (s_ast_list));

    list->next_asynchronous = 0;
    list->and_or = NULL;
    list->next = NULL;

    return list;
}

void ast_list_delete(s_ast_list *list)
{
    if (!list)
        return;

    ast_list_delete(list->next);
    ast_and_or_delete(list->and_or);
    sfree(list);
}

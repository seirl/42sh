#include "ast.h"
#include "smalloc.h"

s_ast_prefix *ast_prefix_new(void)
{
    s_ast_prefix *prefix = smalloc(sizeof (s_ast_prefix));

    prefix->assignment = NULL;
    prefix->redirection = NULL;
    prefix->next = NULL;

    return prefix;
}

void ast_prefix_delete(s_ast_prefix *pref)
{
    if (!pref)
        return;

    ast_prefix_delete(pref->next);
    ast_assignment_delete(pref->assignment);
    ast_redirection_list_delete(pref->redirection);

    sfree(pref);
}

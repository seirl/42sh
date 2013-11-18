#include "ast.h"
#include "smalloc.h"

s_ast_word_list *ast_word_list_new(void)
{
    s_ast_word_list *wl = smalloc(sizeof (s_ast_word_list));

    wl->next = NULL;
    wl->word = NULL;

    return wl;
}

void ast_word_list_delete(s_ast_word_list *wl)
{
    if (!wl)
        return;

    ast_word_list_delete(wl->next);
    ast_compound_word_delete(wl->word);

    sfree(wl);
}

#include "expand.h"
#include "ast.h"

static void add_elt(s_ast_element *elt, s_ast_compound_word *cw)
{
    s_ast_compound_word *it = cw;
    s_ast_element *nelt;
    s_ast_element *prev = elt;
    while (it)
    {
        nelt = ast_element_new();
        nelt->word = ast_compound_word_new();
        nelt->word->word = it->word;
        it->word = NULL;
        if (prev == NULL)
        {
            nelt->next = elt->next;
            elt->next = nelt;
        }
        else
        {
            nelt->next = prev->next;
            prev->next = nelt;
            prev = nelt;
        }
        it = it->next;
    }
    ast_compound_word_delete(cw);
}

void expand_element(s_shell *shell, s_ast_element *elt)
{
    s_ast_element *wl;
    s_ast_compound_word *cw = NULL;
    for (wl = elt; wl; wl = wl->next)
    {
        cw = split_compound_word(shell, wl->word, 0);
        add_elt(wl, cw);
    }
}

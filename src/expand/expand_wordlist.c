#include "expand.h"
#include "ast.h"

static void add_wl(s_ast_word_list *l, s_ast_compound_word *cw)
{
    s_ast_compound_word *it = cw;
    s_ast_word_list *nl;
    s_ast_word_list *prev = l;
    while (it)
    {
        nl = ast_word_list_new();
        nl->word = ast_compound_word_new();
        nl->word->word = it->word;
        it->word = NULL;
        if (prev == NULL)
        {
            nl->next = l->next;
            l->next = nl;
        }
        else
        {
            nl->next = prev->next;
            prev->next = nl;
            prev = nl;
        }
        it = it->next;
    }
    ast_compound_word_delete(cw);
}

void expand_wordlist(s_shell *shell, s_ast_word_list *elt)
{
    s_ast_word_list *wl;
    s_ast_compound_word *cw = NULL;
    for (wl = elt; wl; wl = wl->next)
    {
        cw = split_compound_word(shell, wl->word, 1);
        add_wl(wl, cw);
        return;
    }
}

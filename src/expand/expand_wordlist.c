#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "string_utils.h"
#include "smalloc.h"
#include "ast.h"
#include "lexer.h"
#include "expand_wordlist.h"
#include "expand.h"
#include "env.h"
#include "ifs.h"

static void add_wl(s_ast_word_list *l, s_ast_compound_word *cw)
{
    s_ast_compound_word *it = cw;
    s_ast_compound_word *free_me = NULL;
    s_ast_word_list *nl;
    s_ast_word_list *prev = l;
    while (it)
    {
        nl = smalloc(sizeof (s_ast_word_list));
        nl->word = smalloc(sizeof (s_ast_compound_word));
        nl->word->next = NULL;
        nl->word->word = it->word;
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
        free_me = it->next;
        sfree(it);
        it = free_me;
    }
}

void expand_wordlist(s_shell *shell, s_ast_word_list *elt)
{
    s_ast_word_list *wl;
    s_ast_compound_word *cw = NULL;
    for (wl = elt; wl; wl = wl->next)
    {
        cw = split_compound_word(shell, wl->word);
        add_wl(wl, cw);
        return;
    }
}

#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "string_utils.h"
#include "smalloc.h"
#include "ast.h"
#include "lexer.h"
#include "expand.h"
#include "env.h"
#include "ifs.h"

static void add_elt(s_ast_element *elt, s_ast_compound_word *cw)
{
    s_ast_compound_word *it = cw;
    s_ast_compound_word *free_me = NULL;
    s_ast_element *nelt;
    s_ast_element *prev = elt;
    while (it)
    {
        nelt = scalloc(1, sizeof (s_ast_element));
        nelt->word = smalloc(sizeof (s_ast_compound_word));
        nelt->word->next = NULL;
        nelt->word->word = it->word;
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
        free_me = it->next;
        sfree(it);
        it = free_me;
    }
}

void expand_element(s_shell *shell, s_ast_element *elt)
{
    s_ast_element *wl;
    s_ast_compound_word *cw = NULL;
    for (wl = elt; wl; wl = wl->next)
    {
        cw = split_compound_word(shell, wl->word, 0);
        add_elt(wl, cw);
        //return;
    }
}

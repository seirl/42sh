#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "string_utils.h"
#include "smalloc.h"
#include "ast.h"
#include "lexer.h"
#include "expand_wordlist.h"
#include "env.h"
#include "ifs.h"

static s_ast_compound_word *cw_add_word(s_ast_compound_word *cw, s_string *s)
{
    s_ast_compound_word *ncw = smalloc(sizeof (s_ast_compound_word));
    ncw->word = smalloc(sizeof (s_ast_word));
    ncw->word->kind = WORD;
    ncw->word->str = string_duplicate(s);
    ncw->next = NULL;
    if (cw == NULL)
        return ncw;
    s_ast_compound_word *it;
    for (it = cw; cw->next; cw = cw->next)
        ;
    it->next = ncw;
    return cw;
}

static s_ast_compound_word *split_word(s_shell *shell, s_ast_word *word,
        s_ast_compound_word *cw)
{
    if (word->kind != EXPAND_NAME)
        return NULL;

    char *value = env_get(shell, word->str->buf);
    //printf("ENV get |%s|\n", value);

    //replace current word
    string_reset(word->str);
    int i;
    for (i = 0; value[i] && !is_ifs(shell, value[i]); ++i)
        string_putc(word->str, value[i]);
    word->kind = WORD;
    //printf("Word is now %s\n", word->str->buf);
    for (; value[i] && is_ifs(shell, value[i]); ++i)
        continue;
    if (value[i] == 0) //variable without split
        return NULL;
    --i;

    //fill the garbage words list
    s_string *buf = string_create(0);
    ++i;
    for (; value[i]; ++i)
    {
        if (is_ifs(shell, value[i]))
        {
            //printf("Add %s\n", buf->buf);
            cw = cw_add_word(cw, buf);
            string_reset(buf);
            continue;
        }
        string_putc(buf, value[i]);
    }
    //printf("Add %s\n", buf->buf);
    cw = cw_add_word(cw, buf);
    string_free(buf);
    return cw;
}

static s_ast_compound_word *split_compound_word(s_shell *shell, s_ast_compound_word *cw)
{
    //printf("Split compound\n");
    //TODO merge last
    s_ast_compound_word *it;
    s_ast_compound_word *res = NULL;
    for (it = cw; it; it = it->next)
        res = split_word(shell, it->word, res);
    return res;
}

static void add_wl(s_ast_word_list *l, s_ast_compound_word *cw)
{
    s_ast_compound_word *it = cw;
    s_ast_compound_word *free_me = NULL;
    s_ast_word_list *nl;
    s_ast_word_list *prev = NULL;
    while (it)
    {
        //printf("Garbage %s\n", it->word->str->buf);
        nl = smalloc(sizeof (s_ast_word_list));
        nl->word = smalloc(sizeof (s_ast_compound_word));
        nl->word->next = NULL;
        nl->word->word = it->word;
        if (prev == NULL)
        {
            nl->next = l->next;
            l->next = nl;
            prev = nl;
        }
        else
        {
            nl->next = prev->next;
            prev->next = nl;
        }
        free_me = it->next;
        sfree(it);
        it = free_me;
    }
}

void expand_wordlist(s_shell *shell, s_ast_word_list *elt)
{
    //printf("Expand wordlist\n");
    s_ast_word_list *wl;
    s_ast_compound_word *cw = NULL;
    //s_ast_compound_word *it = NULL;
    for (wl = elt; wl; wl = wl->next)
    {
        cw = split_compound_word(shell, wl->word);
        add_wl(elt, cw);
    }
}

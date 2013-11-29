#include "expand.h"

#include "ast.h"
#include "ifs.h"
#include "smalloc.h"
#include "string_utils.h"

static s_ast_compound_word *cw_add_word(s_ast_compound_word *cw, s_string *s)
{
    s_ast_compound_word *ncw = ast_compound_word_new();
    ncw->word = ast_word_new();
    ncw->word->kind = WORD;
    ncw->word->str = string_duplicate(s);
    if (cw == NULL)
        return ncw;
    s_ast_compound_word *it;
    for (it = cw; it->next; it = it->next)
        continue;
    it->next = ncw;
    return cw;
}

static int replace_current_word(s_shell *shell, char *value, s_ast_word *word)
{
    string_reset(word->str);
    int i;
    for (i = 0; value[i] && !is_ifs(shell, value[i]); ++i)
        string_putc(word->str, value[i]);
    word->kind = WORD;
    for (; value[i] && is_ifs(shell, value[i]); ++i)
        continue;
    return i;
}

static char *expand_word_by_kind(s_shell *shell, s_ast_word *word, int type)
{
    if (type == 1 && word->kind == EXPAND_NAME)
        return string_release(expand_simple_var(shell, word->str));
    if (word->kind == WORD)
    {
        s_string *w = expand_simple_word(shell, word->str);
        return string_release(expand_glob(shell, w));
    }
    if (type == 1 && word->kind == EXPAND_ARITHM)
        return string_release(expand_arithm(shell, word->str));
    if (type == 1 && word->kind == EXPAND_PARAM)
        return string_release(expand_substs_var(shell, word->str));
    return NULL;
}

static s_ast_compound_word *split_word(s_shell *shell, s_ast_word *word,
                                       s_ast_compound_word *cw, int type)
{
    char *value = expand_word_by_kind(shell, word, type);
    if (value == NULL)
        return NULL;
    int i = replace_current_word(shell, value, word);
    if (value[i] == 0)
        return NULL;
    --i;

    s_string *buf = string_create(0);
    ++i;
    for (; value[i]; ++i)
    {
        if (is_ifs(shell, value[i]))
        {
            cw = cw_add_word(cw, buf);
            string_reset(buf);
            continue;
        }
        string_putc(buf, value[i]);
    }
    cw = cw_add_word(cw, buf);
    string_free(buf);
    sfree(value);
    return cw;
}

s_ast_compound_word *split_compound_word(s_shell *shell,
                                         s_ast_compound_word *cw, int type)
{
    //TODO: schischi merge last
    s_ast_compound_word *it;
    s_ast_compound_word *res = NULL;
    for (it = cw; it; it = it->next)
        res = split_word(shell, it->word, res, type);
    return res;
}

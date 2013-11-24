#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"

static s_string *(*const expand_funcs[])(const s_shell *, const s_string *) =
{
    expand_simple_word,
    expand_simple_var,
    expand_substs_var,
    expand_substs_shell,
    NULL,
    expand_squote,
    NULL,
    NULL,
};

static s_string *expand_word(const s_shell *shell, const s_ast_word *word)
{
    string_rewind(word->str);
    return expand_funcs[word->kind](shell, word->str);
}

s_string *expand(const s_shell *shell, const s_ast_compound_word *input)
{
    s_string *r = string_create(0);
    for (const s_ast_compound_word *cw = input; cw; cw = cw->next)
    {
        s_string *expanded_word = expand_word(shell, cw->word);
        string_cat(r, expanded_word);
        string_free(expanded_word);
    }
    return r;
}

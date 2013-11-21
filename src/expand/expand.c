#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"

static s_string *(*const expand_funcs[])(s_shell *, s_string *) =
{
    expand_simple_word,
    expand_substs_shell,
    expand_substs_var,
    expand_simple_var,
};

static s_string *expand_word(s_shell *shell, s_ast_word *word)
{
    return expand_funcs[word->kind](shell, word->str);
}

s_string *expand(s_shell *shell, s_ast_compound_word *input)
{
    s_string *r = string_create(0);
    for (s_ast_compound_word *cw = input; cw; cw = cw->next)
        string_cat(r, expand_word(shell, cw->word));
    return r;
}

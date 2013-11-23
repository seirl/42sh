#include "expand.h"
#include "exec.h"
#include "shell_private.h"
#include "env.h"

s_string *expand_word(s_ast_word *word)
{
    if (word)
        return word->str;
    return NULL;
}

s_string *expand_compound(s_shell *shell, s_ast_compound_word *word)
{
    if (word && word->word)
        return expand(shell, word);
    return NULL;
}

void exec_assignment(s_shell *shell, s_ast_assignment *assign)
{
    s_string *value = string_duplicate(expand_compound(shell, assign->value));
    s_string *name = string_duplicate(assign->name);
    env_set(shell, string_release(name), string_release(value));
}

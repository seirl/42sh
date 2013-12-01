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
    s_string *name = string_duplicate(assign->name);
    if (!assign->name)
        env_set(shell, "", name->buf);
    else
    {
        s_string *value;
        if (assign->value)
            value = expand_compound(shell, assign->value);
        else
            value = string_create_from("");
        env_set(shell, value->buf, name->buf);
        string_free(value);
    }
    string_free(name);
}

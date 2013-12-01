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
    if (!assign->value)
        env_set(shell, "", assign->name->buf);
    else
    {
        s_string *value;
        if (assign->value)
            value = expand_compound(shell, assign->value);
        else
            value = string_create_from("");
        env_set(shell, value->buf, assign->name->buf);
        string_free(value);
    }
}

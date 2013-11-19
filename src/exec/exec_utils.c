#include "exec.h"

void init_shell(void)
{
    shell.builtins = NULL;
    shell.built_count = 0;
    shell.status = 0;
}

s_string *expand_word(s_ast_word *word)
{
    if (word)
        return word->str;
    return NULL;
}

s_string *expand_compound(s_ast_compound_word *word)
{
    if (word && word->word)
        return word->word->str;
    return NULL;
}

void exec_assignment(s_ast_assignment *assign)
{
    s_string *value = expand_compound(assign->value);
    vars_set(assign->name, value);
}

handler builtin_handler(char *name)
{
    if (!shell.builtins || !shell.built_count)
        return NULL;
    for (unsigned int i = 0; i < shell.built_count; ++i)
        if (!strcmp(shell.builtins[i].name, name))
            return shell.builtins[i].callback;
    return NULL;
}

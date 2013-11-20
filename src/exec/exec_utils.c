#include "exec.h"

void init_shell(void)
{
    g_shell.builtins = NULL;
    g_shell.built_count = 0;
    g_shell.status = 0;
    g_shell.curr_argv = NULL;
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
    if (!g_shell.builtins || !g_shell.built_count)
        return NULL;
    for (unsigned int i = 0; i < g_shell.built_count; ++i)
        if (!strcmp(g_shell.builtins[i].name, name))
            return g_shell.builtins[i].callback;
    return NULL;
}

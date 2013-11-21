#include "exec.h"

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
    s_string *value = string_duplicate(expand_compound(assign->value));
    s_string *name = string_duplicate(assign->name);
    vars_set(name, value);
}

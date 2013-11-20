#include "ast.h"
#include "smalloc.h"

s_ast_assignment *ast_assignment_new(void)
{
    s_ast_assignment *assignment = smalloc(sizeof (s_ast_assignment));

    assignment->name = NULL;
    assignment->value = NULL;

    return assignment;
}

void ast_assignment_delete(s_ast_assignment *ass)
{
    if (!ass)
        return;

    if (ass->name)
        string_free(ass->name);
    ast_compound_word_delete(ass->value);
    sfree(ass);
}

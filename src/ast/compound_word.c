#include "ast.h"
#include "smalloc.h"

s_ast_compound_word *ast_compound_word_new(void)
{
    s_ast_compound_word *cw = smalloc(sizeof (s_ast_compound_word));

    cw->word = NULL;
    cw->next = NULL;

    return cw;
}

void ast_compound_word_delete(s_ast_compound_word *cw)
{
    if (!cw)
        return;
    ast_compound_word_delete(cw->next);
    ast_word_delete(cw->word);
}

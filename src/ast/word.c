#include "ast.h"
#include "smalloc.h"

s_ast_word *ast_word_new(void)
{
    s_ast_word *word = smalloc(sizeof (s_ast_word));

    word->kind = 0;
    word->str = NULL;

    return word;
}

void ast_word_delete(s_ast_word *word)
{
    if (!word)
        return;
    if (word->str)
        string_free(word->str);
    sfree(word);
}

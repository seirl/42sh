#include "ast.h"
#include "smalloc.h"

s_ast_element *ast_element_new(void)
{
    s_ast_element *element = smalloc(sizeof (s_ast_element));

    element->next = NULL;
    element->word = NULL;
    element->redirection = NULL;

    return element;
}

void ast_element_delete(s_ast_element *elem)
{
    if (!elem)
        return;

    ast_element_delete(elem->next);
    ast_compound_word_delete(elem->word);
    ast_redirection_list_delete(elem->redirection);

    sfree(elem);
}

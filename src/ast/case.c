#include "ast.h"
#include "smalloc.h"

s_ast_case *ast_case_new(void)
{
    s_ast_case *mycase = smalloc(sizeof (s_ast_case));

    mycase->word = NULL;
    mycase->clauses = NULL;

    return mycase;
}

s_ast_case_item *ast_case_item_new(void)
{
    s_ast_case_item *item = smalloc(sizeof (s_ast_case_item));

    item->next = NULL;
    item->match = NULL;
    item->cmd_list = NULL;

    return item;
}

void ast_case_item_delete(s_ast_case_item *item)
{
    if (!item)
        return;

    ast_case_item_delete(item->next);
    ast_word_list_delete(item->match);
    ast_list_delete(item->cmd_list);

    sfree(item);
}

void ast_case_delete(s_ast_case *casee)
{
    if (!casee)
        return;

    ast_compound_word_delete(casee->word);
    ast_case_item_delete(casee->clauses);

    sfree(casee);
}

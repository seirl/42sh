#include "ast.h"
#include "smalloc.h"

void ast_case_item_delete(s_ast_case_item *item)
{
    if (!item)
        return;

    ast_word_list_delete(item->matches);
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

#include "exec.h"

void exec_if_node(struct if_node *node)
{
    exec_node(node->cond);
    if (status == 0)
        exec_node(node->if_true);
    else
        exec_node(node->if_false);
}

void exec_for_node(struct for_node *node)
{
    for (int i = 0; node->val[i] != NULL; ++i)
    {
        /* FIXME add_var(g_42sh->vars, node->name, node->val[i]); */
        exec_node(node->statement);
    }
}

void exec_while_node(struct while_node *node)
{
    exec_node(node->cond);
    while (status != 0)
    {
        exec_node(node->statement);
        exec_node(node->cond);
    }
}

void exec_case_node(struct case_node *node)
{
#if 0
    assert(node->var && node->first_case);
    /* -> expansion here <- */
    struct case_switch curr_case = node->first_case;
    curr_case = curr_case->next_case;
    do
    {
        for (int i = 0; curr_case->values[i]; ++i)
            if (my_fnmatch(node->var, curr_case->values[i]) == 0)
                return exec_node(curr_case->statement);
        curr_case = curr_case->next_case;
    } while (curr_case->next_case)
    if (curr_case)
        for (int i = 0; curr_case->values[i]; ++i)
            if (my_fnmatch(node->var, curr_case->values[i]) == 0)
                return exec_node(curr_case->statement);
#endif
    node = node - 0;
    assert(0);
}

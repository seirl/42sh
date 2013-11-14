#include "exec.h"

void exec_or_node(struct binary_node *node)
{
    exec_node(node->left);
    /*
     * if status != 0: cond false
     * if status == 0: cond true
     */
    if (status)
        exec_node(node->right);
    /*
     * if status == 0, cond == true
     * then: pass until an AND node is found
     * which might set global condition value to false
     */
    else
    {
        while (node->right->type == OR)
            node = &node->right->next.or_n;
        /*
         * case:
         * A || B || C && (D ...)
         * if A then directly go to && token and eval D (right of &&)
         */
        if (node->right->type == AND)
            exec_node(node->right->next.and_n.right);
    }
}

void exec_and_node(struct binary_node *node)
{
    exec_node(node->left);
    /*
     * if status != 0: cond false
     * if status == 0: cond true
     */
    if (status == 0)
        exec_node(node->right);
    /*
     * if status != 0, cond == false
     * then: pass until an OR node is found
     * which might set global condition value to true
     */
    else
    {
        while (node->right->type == AND)
        node = &node->right->next.and_n;
        /*
         * case:
         * A && B && C || (D ...)
         * if A then directly go to || token and eval D (right of ||)
         */
        if (node->right->type == OR)
            exec_node(node->right->next.or_n.right);
    }
}

#include "exec.h"

void add_func(char *name, struct ast_node *body)
{
    /* FIXME */
    name += 0;
    body += 0;
    assert(0);
}

void exec_func_node(struct func_node *node)
{
    add_func(node->name, node->body);
    node->name = NULL;
    node->body = NULL;
}

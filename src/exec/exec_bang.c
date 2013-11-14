#include "exec.h"

void exec_bang_node(struct binary_node *node)
{
    assert(node->left);
    exec_node(node->left);
    status = !status;
}

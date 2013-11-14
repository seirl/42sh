#include "exec.h"

int status = 0;

void exec_node2(struct ast_node *node)
{
    switch (node->type)
    {
        case WHILE:
            exec_while_node(&node->next.while_n);
            break;
        case BANG:
            exec_bang_node(&node->next.bang_n);
            break;
        case OR:
            exec_or_node(&node->next.or_n);
            break;
        case AND:
            exec_and_node(&node->next.and_n);
            break;
        case REDIR:
            exec_redir_node(&node->next.redir_n);
            break;
        default:
            fprintf(stderr, "Wrong node type or not implemented.\n");
    }
}

void exec_node(struct ast_node *node)
{
    assert(node);
    switch (node->type)
    {
        case PIPE:
            exec_pipe_node(&node->next.pipe_n);
            break;
        case CMD:
            exec_cmd_node(&node->next.cmd_n);
            break;
        case FUNC:
            exec_func_node(&node->next.func_n);
            break;
        case IF:
            exec_if_node(&node->next.if_n);
            break;
        case FOR:
            exec_for_node(&node->next.for_n);
            break;
        case CASE:
            exec_case_node(&node->next.case_n);
            break;
        default:
            exec_node2(node);
    }
}

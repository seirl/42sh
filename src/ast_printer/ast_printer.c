#include <stdio.h>
#include "ast.h"
#include "ast_printer.h"

unsigned long int ph(void *ptr)
{
    char *ptr_c = ptr;
    char *ptr_n = NULL;
    return ptr_c - ptr_n;
}

void print_pipeline(s_ast_pipeline *node, void *prev, FILE *out)
{
    if (!node)
        return;
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(node));
    for (s_ast_pipeline *p = node; p; p = p->next)
    {
        fprintf(out, "node_%lu [label=\"Pipeline\"];\n", ph(p));
        if (p->next)
            fprintf(out, "node_%lu -> node_%lu [label=\"||\"];\n", ph(p),
                    ph(p->next));
        print_cmd(p->cmd, p, out);
    }
}

void print_cmd_list(s_ast_list *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label=\"cmd_list\"]\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    for (s_ast_and_or *ao = n->and_or; ao; ao = ao->next)
    {
        fprintf(out, "node_%lu [label=\"and_or\"];\n", ph(ao));
        print_pipeline(ao->pipeline, ao, out);
        if (ao->next)
            fprintf(out, "node_%lu -> node_%lu [label=\"%s\"];\n",
                    ph(ao), ph(ao->next), ao->and_or ? "||" : "&&");
    }
    print_cmd_list(n->next, prev, out);
}

void print_ast(s_ast_input *in, FILE *out)
{
    if (!in)
        return;
    fprintf(out, "digraph G {\n");
    fprintf(out, "node_0 [label=\"entry\"];\n");
    print_cmd_list(in->list, 0, out);
    fprintf(out, "}\n");
}

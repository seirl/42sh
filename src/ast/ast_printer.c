#include <stdio.h>
#include "ast.h"

static void print_pipeline(s_ast_and_or *node, FILE *out)
{
    if (!node->pipeline)
        return;
    fprintf(out, "and_or_%p -> pipeline_%p;\n", node, node->pipeline);
    for (s_ast_pipeline *p = node->pipeline; p; p = p->next)
    {
        fprintf(out, "pipeline_%p [label=\"Pipeline\"];\n");
        if (p->next)
            fprintf(out, "pipeline_%p -> pipeline_%p;\n", p, p->next);
    }
}

void print_ast(s_ast_input *in, FILE *out)
{
    fprintf(out, "digraph G {\n");
    for (s_ast_list *l = in->list; l; l = l->next)
    {
        fprintf(out, "subgraph clusterl%p {\n", l);
        for (s_ast_and_or *ao = l->and_or; ao; ao = ao->next)
        {
            fprintf(out, "and_or_%p [label=\"and_or\"];\n", ao);
            print_pipeline(ao, out);
            if (ao->next)
                fprintf(out, "and_or_%p -> and_or_%p [label=\"%s\"];\n",
                        ao, ao->next, ao->and_or ? "||" : "&&");
        }
        fprintf(out, "}\n");
        fprintf(out, "entry -> clusterl%p;\n", l);
    }
    fprintf(out, "}\n");
}

#include <stdio.h>
#include "ast.h"

void print_word(s_ast_word *w, void *prev, FILE *out)
{
    if (!w)
        return;
    //TODO : print_type
    fprintf(out, "node_%p [label=\"word '%s'\"];", w, w->str->buf);
    fprintf(out, "node_%p -> node_%p;", prev, w);
}

void print_compound_word(s_ast_compound_word *w, void *prev, FILE *out)
{
    if (!w)
        return;
    fprintf(out, "node_%p [label=\"compound_word\"];", w);
    fprintf(out, "node_%p -> node_%p;", prev, w);
    print_compound_word(w->next, prev, out);
}

void print_assignment(s_ast_assignment *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"assignment '%s'\"];", n, n->name->buf);
    fprintf(out, "node_%p -> node_%p;", prev, n);
    print_compound_word(n->value, n, out);
}

void print_prefix(s_ast_prefix *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"compound_word\"];", n);
    fprintf(out, "node_%p -> node_%p;", prev, n);
    print_assignment(n->assignment, n, out);
    // TODO: print_redirection_list
    print_prefix(n->next, prev, out);
}

void print_simple_cmd(s_ast_simple_cmd *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"simple_cmd\"];", n);
    fprintf(out, "node_%p -> node_%p;", prev, n);
    print_prefix(n->prefixes, n, out);
}

void print_ast_element(s_ast_element *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"ast_element\"];", n);
    fprintf(out, "node_%p -> node_%p;", prev, n);
    print_compound_word(n->word, n, out);
    //TODO: print_redirection_list
    print_ast_element(n->next, prev, out);
}

void print_shell_cmd(s_ast_shell_cmd *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"shell_cmd %s\"];", n,
            (n->subshell) ? "(subshell)" : "");
    fprintf(out, "node_%p -> node_%p;", prev, n);
}

void print_func_dec(s_ast_funcdec *n, void *prev, FILE *out)
{
}

void print_redirs(s_ast_redirection_list *n, void *prev, FILE *out)
{
}

void print_cmd(s_ast_cmd *c, void *prev, FILE *out)
{
    fprintf(out, "node_%p [label=\"cmd\"];", c);
    fprintf(out, "node_%p -> node_%p;", prev, c);
    if (c->simple_cmd)
        print_simple_cmd(c->simple_cmd, c, out);
    else if (c->shell_cmd)
        print_shell_cmd(c->shell_cmd, c, out);
    else if (c->func_dec)
        print_func_dec(c->func_dec, c, out);
    else if (c->redirections)
        print_redirs(c->redirections, c, out);
}

void print_pipeline(s_ast_pipeline *node, void *prev, FILE *out)
{
    fprintf(out, "node_%p -> node_%p;\n", prev, node);
    for (s_ast_pipeline *p = node; p; p = p->next)
    {
        fprintf(out, "node_%p [label=\"Pipeline\"];\n", p);
        if (p->next)
            fprintf(out, "node_%p -> node_%p [label=\"||\"];\n", p, p->next);
        print_cmd(p->cmd, p, out);
    }
}

void print_ast(s_ast_input *in, FILE *out)
{
    if (!in)
        return;
    fprintf(out, "digraph G {\n");
    for (s_ast_list *l = in->list; l; l = l->next)
    {
        fprintf(out, "node_%p [label=\"input\"]\n", l);
        fprintf(out, "entry -> node_%p;\n", l);
        for (s_ast_and_or *ao = l->and_or; ao; ao = ao->next)
        {
            fprintf(out, "node_%p [label=\"and_or\"];\n", ao);
            print_pipeline(ao->pipeline, ao, out);
            if (ao->next)
                fprintf(out, "node_%p -> node_%p [label=\"%s\"];\n",
                        ao, ao->next, ao->and_or ? "||" : "&&");
        }
    }
    fprintf(out, "}\n");
}

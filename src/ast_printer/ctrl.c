#include <stdio.h>
#include "ast.h"
#include "ast_printer.h"

static void print_ast_case_item(s_ast_case_item *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label = \"case_item\"];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_word_list(n->matches, n, out);
    print_cmd_list(n->cmd_list, n, out);
}

void print_ast_case(s_ast_case *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label = \"case\", shape = Mdiamond, "
            "style = filled, fillcolor = palegreen];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_compound_word(n->word, n, out);
    print_ast_case_item(n->clauses, n, out);
}

void print_ast_for(s_ast_for *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label = \"for\", shape = Mdiamond, style = filled,"
                 " fillcolor = palegreen];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_word(n->identifier, n, out);
    print_word_list(n->values, n, out);
    print_cmd_list(n->cmd_list, n, out);
}

static void print_ast_else(s_ast_else *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label = \"else\", shape = Mdiamond, "
            "style = filled, fillcolor = palegreen];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_cmd_list(n->elif_predicate, n, out);
    print_cmd_list(n->elif_cmds, n, out);
    print_cmd_list(n->else_cmds, n, out);
    print_ast_else(n->next_else, prev, out);
}

void print_ast_if(s_ast_if *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label = \"if\", shape = Mdiamond, style = filled, "
                 "fillcolor = palegreen];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_cmd_list(n->predicate, n, out);
    print_cmd_list(n->then_cmds, n, out);
    print_ast_else(n->else_clause, n, out);
}

void print_ast_while(s_ast_while *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label = \"while\", shape = Mdiamond, "
            "style = filled, fillcolor = palegreen];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_cmd_list(n->predicate, n, out);
    print_cmd_list(n->cmds, n, out);
}

void print_ast_until(s_ast_until *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label = \"until\", shape = Mdiamond, "
            "style = filled, fillcolor = palegreen];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_cmd_list(n->predicate, n, out);
    print_cmd_list(n->cmds, n, out);
}

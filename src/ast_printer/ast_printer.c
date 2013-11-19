#include <stdio.h>
#include "ast.h"
#include "ast_printer.h"

static const char *redirection_type_str[] =
{
    ">",
    ">>",
    "<",
    "<<",
    "<<-",
    "<&",
    ">&",
    ">|",
    "<>",
};

static const char *word_kind_str[] =
{
    "WORD",
    "SUBSTS_SHELL",
    "SUBSTS_VAR",
    "EXPAND_VAR",
};

void print_word(s_ast_word *w, void *prev, FILE *out)
{
    if (!w)
        return;
    fprintf(out, "node_%p [label=\"word '%s' (%s)\"];\n", w, w->str->buf,
            word_kind_str[w->kind]);
    fprintf(out, "node_%p -> node_%p;\n", prev, w);
}

void print_compound_word(s_ast_compound_word *w, void *prev, FILE *out)
{
    if (!w)
        return;
    fprintf(out, "node_%p [label=\"compound_word\"];\n", w);
    fprintf(out, "node_%p -> node_%p;\n", prev, w);
    print_word(w->word, w, out);
    print_compound_word(w->next, prev, out);
}

void print_word_list(s_ast_word_list *w, void *prev, FILE *out)
{
    if (!w)
        return;
    fprintf(out, "node_%p [label=\"word_list\"];\n", w);
    fprintf(out, "node_%p -> node_%p;\n", prev, w);
    print_compound_word(w->word, w, out);
    print_word_list(w->next, prev, out);
}

void print_heredoc(s_ast_heredoc *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"heredoc '%s'\"];\n", n, n->heredoc->buf);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
}

void print_assignment(s_ast_assignment *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"assignment '%s'\"];\n", n, n->name->buf);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_compound_word(n->value, n, out);
}

void print_redirection_list(s_ast_redirection_list *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"redirection '%s' (%d)\"];\n", n,
            redirection_type_str[n->type], n->io->io_number);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_compound_word(n->word, n, out);
    print_compound_word(n->heredoc_delim, n, out);
    print_heredoc(n->heredoc, n, out);
    print_redirection_list(n->next, prev, out);
}

void print_prefix(s_ast_prefix *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"compound_word\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_assignment(n->assignment, n, out);
    print_redirection_list(n->redirection, n, out);
    print_prefix(n->next, prev, out);
}

void print_element(s_ast_element *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"element\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_compound_word(n->word, n, out);
    print_redirection_list(n->redirection, n, out);
    print_element(n->next, prev, out);
}

void print_simple_cmd(s_ast_simple_cmd *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"simple_cmd\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_prefix(n->prefixes, n, out);
    print_element(n->elements, n, out);
}

void print_ast_case_item(s_ast_case_item *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"case_item\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_word_list(n->matches, n, out);
    print_cmd_list(n->cmd_list, n, out);
}

void print_ast_case(s_ast_case *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"case\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_compound_word(n->word, n, out);
    print_ast_case_item(n->clauses, n, out);
}

void print_ast_for(s_ast_for *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"for\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_word(n->identifier, n, out);
    print_word_list(n->values, n, out);
    print_cmd_list(n->cmd_list, n, out);
}

void print_ast_else(s_ast_else *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"else\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_cmd_list(n->elif_predicate, n, out);
    print_cmd_list(n->elif_cmds, n, out);
    print_cmd_list(n->else_cmds, n, out);
    print_ast_else(n->next_else, prev, out);
}

void print_ast_if(s_ast_if *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"if\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_cmd_list(n->predicate, n, out);
    print_cmd_list(n->then_cmds, n, out);
}

void print_ast_while(s_ast_while *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"while\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_cmd_list(n->predicate, n, out);
    print_cmd_list(n->cmds, n, out);
}

void print_ast_until(s_ast_until *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"until\"];\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_cmd_list(n->predicate, n, out);
    print_cmd_list(n->cmds, n, out);
}

void print_shell_cmd(s_ast_shell_cmd *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"shell_cmd '%s'\"];\n", n,
            (n->subshell) ? "(subshell)" : "");
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    if (n->cmd_list)
        print_cmd_list(n->cmd_list, n, out);
#define X(U1, Type, Sub, U2)                                    \
    else if (n->ctrl_structure == Type)                         \
        print_ ## Sub(n->ctrl.Sub, n, out);
# include "shell_command.def"
#undef X
}

void print_func_dec(s_ast_funcdec *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"funcdec '%s'\"];\n", n, n->name->buf);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    print_shell_cmd(n->shell_cmd, n, out);
}

void print_cmd(s_ast_cmd *c, void *prev, FILE *out)
{
    if (!c)
        return;
    fprintf(out, "node_%p [label=\"cmd\"];\n", c);
    fprintf(out, "node_%p -> node_%p;\n", prev, c);
    if (c->simple_cmd)
        print_simple_cmd(c->simple_cmd, c, out);
    else if (c->shell_cmd)
        print_shell_cmd(c->shell_cmd, c, out);
    else if (c->func_dec)
        print_func_dec(c->func_dec, c, out);
    print_redirection_list(c->redirections, c, out);
}

void print_pipeline(s_ast_pipeline *node, void *prev, FILE *out)
{
    if (!node)
        return;
    fprintf(out, "node_%p -> node_%p;\n", prev, node);
    for (s_ast_pipeline *p = node; p; p = p->next)
    {
        fprintf(out, "node_%p [label=\"Pipeline\"];\n", p);
        if (p->next)
            fprintf(out, "node_%p -> node_%p [label=\"||\"];\n", p, p->next);
        print_cmd(p->cmd, p, out);
    }
}

void print_cmd_list(s_ast_list *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%p [label=\"cmd_list\"]\n", n);
    fprintf(out, "node_%p -> node_%p;\n", prev, n);
    for (s_ast_and_or *ao = n->and_or; ao; ao = ao->next)
    {
        fprintf(out, "node_%p [label=\"and_or\"];\n", ao);
        print_pipeline(ao->pipeline, ao, out);
        if (ao->next)
            fprintf(out, "node_%p -> node_%p [label=\"%s\"];\n",
                    ao, ao->next, ao->and_or ? "||" : "&&");
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

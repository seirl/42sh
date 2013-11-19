#include <stdio.h>
#include "ast.h"
#include "ast_printer.h"

void print_assignment(s_ast_assignment *n, void *prev, FILE *out)
{
    if (!n)
        return;
    s_string *c = clean(n->name);
    fprintf(out, "node_%lu [label=\"assignment '%s'\"];\n", ph(n),
            c->buf);
    string_free(c);
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_compound_word(n->value, n, out);
}

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
void print_redirection_list(s_ast_redirection_list *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label=\"redirection '%s'\"];\n", ph(n),
            redirection_type_str[n->type]);
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_compound_word(n->word, n, out);
    print_compound_word(n->heredoc_delim, n, out);
    print_heredoc(n->heredoc, n, out);
    print_redirection_list(n->next, prev, out);
}

void print_prefix(s_ast_prefix *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label=\"compound_word\"];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_assignment(n->assignment, n, out);
    print_redirection_list(n->redirection, n, out);
    print_prefix(n->next, prev, out);
}

void print_element(s_ast_element *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label=\"element\"];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_compound_word(n->word, n, out);
    print_redirection_list(n->redirection, n, out);
    print_element(n->next, prev, out);
}

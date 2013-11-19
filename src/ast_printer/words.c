#include <stdio.h>
#include "ast.h"
#include "ast_printer.h"

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
    s_string *c = clean(w->str);
    fprintf(out, "node_%lu [label=\"word '%s' (%s)\", shape=triangle];\n", ph(w),
            c->buf, word_kind_str[w->kind]);
    string_free(c);
    fprintf(out, "node_%lu -> node_%lu [color=blue];\n", ph(prev), ph(w));
}

void print_compound_word(s_ast_compound_word *w, void *prev, FILE *out)
{
    if (!w)
        return;
    fprintf(out, "node_%lu [label=\"compound_word\"];\n", ph(w));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(w));
    print_word(w->word, w, out);
    print_compound_word(w->next, prev, out);
}

void print_word_list(s_ast_word_list *w, void *prev, FILE *out)
{
    if (!w)
        return;
    fprintf(out, "node_%lu [label=\"word_list\"];\n", ph(w));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(w));
    print_compound_word(w->word, w, out);
    print_word_list(w->next, prev, out);
}

void print_heredoc(s_ast_heredoc *n, void *prev, FILE *out)
{
    if (!n)
        return;

    s_string *c = clean(n->heredoc);
    fprintf(out, "node_%lu [label=\"heredoc '%s'\"];\n", ph(n),
            c->buf);
    string_free(c);
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
}

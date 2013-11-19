#include <stdio.h>
#include "ast.h"
#include "ast_printer.h"

void print_simple_cmd(s_ast_simple_cmd *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label=\"simple_cmd\"];\n", ph(n));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_prefix(n->prefixes, n, out);
    print_element(n->elements, n, out);
}

void print_shell_cmd(s_ast_shell_cmd *n, void *prev, FILE *out)
{
    if (!n)
        return;
    fprintf(out, "node_%lu [label=\"shell_cmd '%s'\"];\n", ph(n),
            (n->subshell) ? "(subshell)" : "");
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
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
    s_string *c = clean(n->name);
    fprintf(out, "node_%lu [label=\"funcdec '%s'\"];\n", ph(n), c->buf);
    string_free(c);
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(n));
    print_shell_cmd(n->shell_cmd, n, out);
}

void print_cmd(s_ast_cmd *c, void *prev, FILE *out)
{
    if (!c)
        return;
    fprintf(out, "node_%lu [label=\"cmd\"];\n", ph(c));
    fprintf(out, "node_%lu -> node_%lu;\n", ph(prev), ph(c));
    if (c->simple_cmd)
        print_simple_cmd(c->simple_cmd, c, out);
    else if (c->shell_cmd)
        print_shell_cmd(c->shell_cmd, c, out);
    else if (c->func_dec)
        print_func_dec(c->func_dec, c, out);
    print_redirection_list(c->redirections, c, out);
}

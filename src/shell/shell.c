#include "shell_private.h"

#include "ast_printer.h"
#include "exec.h"
#include "shopt.h"
#include "smalloc.h"

s_shell *shell_new(void)
{
    s_shell *shell = smalloc(sizeof (s_shell));

    shell->env = hashtbl_init(hash_char, cmp_char, free_char, free_char);

    shell->builtins = NULL;
    shell->builtins_count = 0;
    shell->curr_argv = NULL;
    shell->parser = NULL;
    shell->repeat = SHELL_STOP;
    shell->status = 0;

    return shell;
}

void shell_setup(s_shell *shell, s_parser *parser, e_shell_repeat repeat)
{
    assert(!shell->parser && "There is already a parser setup.");
    shell->parser = parser;
    shell->repeat = repeat;
}

static int shell_read_eval(s_shell *shell)
{
    s_ast_input *ast;
    ast = parse_rule_input(shell->parser);
    if (parser_diagnostic(shell->parser) && ast)
    {
        if (shopt_get("ast_print"))
            print_ast(ast, stdout);
        exec_ast_input(shell, ast);
    }
    ast_input_delete(ast);
    return 0;
}

int shell_loop(s_shell *shell)
{
    int ret;
    do {
        ret = shell_read_eval(shell);
    } while (shell->repeat);
    return ret;
}

void shell_delete(s_shell *shell)
{
    hashtbl_free(shell->env);
    // TODO: free shell->builtins;

    sfree(shell);
}

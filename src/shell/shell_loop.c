#include "shell_private.h"

#include "ast_printer.h"
#include "exec.h"
#include "shopt.h"

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
    if (!ast)
        shell->state = SHELL_STOP;
    ast_input_delete(ast);
    return 0;
}

int shell_loop(s_shell *shell)
{
    int ret;
    do {
        ret = shell_read_eval(shell);
    } while (shell->state != SHELL_STOP);
    return ret;
}


#include "shell_private.h"

#include "ast_printer.h"
#include "exec.h"
#include "shopt.h"

static e_shell_status shell_read_eval(s_shell *shell)
{
    e_parser_status status;
    s_ast_input *ast;

    if (!parser_ready(shell->parser))
        return SHELL_STOP;

    ast = parse_rule_input(shell->parser);
    status = parser_diagnostic(shell->parser);
    if (status == PARSE_ERROR)
    {
        shell_status_set(shell, 2);
        return shell->state == SHELL_STOP_ON_ERROR ? SHELL_STOP : SHELL_OK;
    }
    else if (ast)
    {
        if (shopt_get(shell, "ast_print"))
            print_ast(ast, stdout);
        exec_ast_input(shell, ast);
    }
    ast_input_delete(ast);
    return SHELL_OK;
}

e_shell_status shell_loop(s_shell *shell)
{
    e_shell_status ret;
    do {
        ret = shell_read_eval(shell);
    } while (ret == SHELL_OK);
    return shell_status(shell);
}

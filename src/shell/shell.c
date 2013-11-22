#include "shell_private.h"

#include "ast_printer.h"
#include "env_private.h"
#include "functions_private.h"
#include "exec.h"
#include "shopt.h"
#include "smalloc.h"

s_shell *shell_new(void)
{
    s_shell *shell = smalloc(sizeof (s_shell));

    env_create(shell);
    functions_init(shell);

    shell->builtins = NULL;
    shell->builtins_count = 0;
    shell->curr_argv = NULL;
    shell->parser = NULL;
    shell->state = SHELL_REPEAT;
    shell->status = 0;

    return shell;
}

void shell_setup(s_shell *shell, s_parser *parser)
{
    assert(!shell->parser && "There is already a parser setup.");
    shell->parser = parser;
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

void shell_delete(s_shell *shell)
{
    env_free(shell);
    functions_free(shell);
    // TODO: free shell->builtins;

    sfree(shell);
}

#include <stdlib.h>
#include "options.h"
#include "shopt.h"
#include "lexer.h"
#include "parser.h"
#include "input.h"
#include "log.h"
#include "ast_printer.h"
#include "smalloc.h"
#include "exec.h"
#include "token.h"
#include "res_ctrl.h"
#include "env.h"
#include "history.h"

static int parse_input(s_lexer *lexer)
{
    s_parser *parser = parser_create(lexer);
    s_ast_input *ast;
    ast = parse_rule_input(parser);
    if (parser_diagnostic(parser) && ast)
    {
        if (shopt_get("ast_print"))
            print_ast(ast, stdout);
        exec_ast_input(ast);
    }
    ast_input_delete(ast);
    parser_delete(parser);
    return 0;
}

int main(int argc, char *argv[])
{
    char *file = NULL;
    char *cmd = NULL;
    int repeat = 0;
    int ret = parse_options(argc, argv, &cmd, &file);
    if (ret == 2)
        return ret;
    if (ret == 1)
        rc_file_load();
    init_shell();
    do {
        s_lexer *lexer = input_to_lexer(cmd, file, &repeat);
        if (lexer == NULL || parse_input(lexer))
            break;
        input_free(lexer, cmd, file);
    } while (repeat);
    history_close();
    shell_free();
    env_free();
    smalloc_clean();
    return EXIT_SUCCESS;
}

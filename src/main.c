#include <stdlib.h>

#include "history.h"
#include "input.h"
#include "lexer.h"
#include "options.h"
#include "parser.h"
#include "res_ctrl.h"
#include "shopt.h"
#include "smalloc.h"
#include "sighandler.h"
#include "env.h"

int main(int argc, char *argv[])
{
    s_shell *shell = shell_new();
    signal_init();
    //FIXME: default var ?
    env_set(shell, "42sh$ ", "PS1");
    env_set(shell, "> ", "PS2");
    char *src = NULL;
    int ret = parse_options(shell, argc, argv, &src);
    if (ret & E_RET)
        return E_RET;
    if (ret & E_ERROR)
        return E_ERROR;
    if (ret & E_RC)
        rc_file_load(shell);

    s_input *input;
    if (!(input = input_create(shell, src, ret)))
        return EXIT_FAILURE; // XXX: Use error specific return code?
    s_lexer *lexer = lex_create(shell, input, LEX_ALL);
    s_parser *parser = parser_create(lexer);
    shell_setup(shell, parser);

    ret = shell_loop(shell);

    parser_delete(parser);
    lex_delete(lexer);
    shell_delete(shell);

    smalloc_clean();
    return ret;
}

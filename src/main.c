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
    env_set(shell, argv[0], "0");

    char *src = NULL;
    int ret = parse_options(shell, argc, argv, &src);
    if (ret & E_RET)
        return 0;
    if (ret & E_ERROR)
        return E_ERROR;
    if (ret & E_RC)
        rc_file_load(shell);

    s_input *input;
    if (!(input = input_create(shell, src, ret)))
        return 127;
    s_lexer *lexer = lex_create(shell, input, LEX_ALL);
    s_parser *parser = parser_create(lexer);
    shell_setup(shell, parser);

    ret = shell_loop(shell);

    parser_delete(parser);
    lex_delete(lexer);
    shell_delete(shell);
    input_destroy(input);

    smalloc_clean();
    return ret;
}

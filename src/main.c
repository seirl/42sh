#include <stdlib.h>

#include "history.h"
#include "input.h"
#include "lexer.h"
#include "options.h"
#include "parser.h"
#include "res_ctrl.h"
#include "shopt.h"
#include "smalloc.h"

int main(int argc, char *argv[])
{
    char *file = NULL;
    char *cmd = NULL;
    e_shell_repeat repeat = 0;
    int ret = parse_options(argc, argv, &cmd, &file);
    if (ret == 3)
        return 0;
    if (ret == 2)
        return ret;
    if (ret == 1)
        rc_file_load();

    s_shell *shell = shell_new();
    s_input *input;
    if (!(input = input_create(shell, &cmd, file, &repeat)))
        return EXIT_FAILURE; // XXX: Use error specific return code?
    s_lexer *lexer = lex_create(input);
    if (!lexer)
        return EXIT_FAILURE; // XXX: Use error specific return code?

    s_parser *parser = parser_create(lexer);
    shell_setup(parser, repeat);

    ret = shell_loop(shell);

    shell_delete(shell);
    history_close();
    smalloc_clean();
    return ret;
}

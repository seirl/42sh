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
    s_shell *shell = shell_new();
    char *file = NULL;
    char *cmd = NULL;
    int ret = parse_options(shell, argc, argv, &cmd, &file);
    if (ret == 3)
        return 0;
    if (ret == 2)
        return ret;
    if (ret == 1)
        rc_file_load(shell);

    s_input *input;
    if (!(input = input_create(shell, &cmd, file)))
        return EXIT_FAILURE; // XXX: Use error specific return code?
    s_lexer *lexer = lex_create(input);
    s_parser *parser = parser_create(lexer);
    shell_setup(shell, parser);

    ret = shell_loop(shell);

    parser_delete(parser);
    lex_delete(lexer);
    input_destroy(input);
    shell_delete(shell);

    smalloc_clean();
    return ret;
}

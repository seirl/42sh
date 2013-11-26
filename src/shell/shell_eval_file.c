#include "exec.h"
#include "input_file.h"
#include "lexer.h"
#include "log.h"
#include "parser.h"
#include "shell.h"

e_shell_status shell_eval_file(s_shell *shell, FILE *f, const char *filename)
{
    s_input *input = input_file_create(f, filename);
    s_lexer *lexer = lex_create(shell, input, LEX_ALL);
    s_parser *parser = parser_create(lexer);
    e_shell_status ret;

    shell_setup(shell, parser);
    ret = shell_loop(shell);

    parser_delete(parser);
    lex_delete(lexer);
    input_destroy(input);
    return ret;
}

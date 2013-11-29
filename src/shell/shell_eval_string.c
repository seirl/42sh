#include "exec.h"
#include "input_string.h"
#include "lexer.h"
#include "log.h"
#include "parser.h"
#include "shell.h"

e_shell_status shell_eval_str(s_shell *shell, const char *str)
{
    s_input *input = input_string_create(string_create_from(str), "str");
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

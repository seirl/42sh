#include "exec.h"
#include "input_file.h"
#include "lexer.h"
#include "log.h"
#include "parser.h"
#include "shell.h"

int shell_eval_file(s_shell *shell, FILE *f, const char *filename)
{
    s_input *input = input_file_create(f, filename);
    s_lexer *lexer = lex_create(input);
    s_parser *parser = parser_create(lexer);
    s_ast_input *ast;
    if ((ast = parse_rule_input(parser)))
    {
        if (parser_diagnostic(parser))
            exec_ast_input(shell, ast);
    }
    parser_delete(parser);
    lex_delete(lexer);
    input_destroy(input);
    return 1;
}

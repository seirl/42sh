#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "string_utils.h"
#include "input.h"
#include "input_string.h"
#include "input_file.h"
#include "lexer_private.h"
#include "readline.h"
#include "macros.h"
#include "log.h"
#include "smalloc.h"

s_lexer *input_to_lexer(char *cmd, char *file, int *repeat)
{
    s_string *input;
    FILE *f;
    if (cmd)
    {
        input = string_create_from(cmd);
        return lex_create(input_string_getc, input_string_topc, input, "cmd");
    }
    if (file)
    {
        if ((f = fopen(file, "r")) == NULL)
            RET_WITH(NULL, PROGNAME": %s: No such file or directory\n", file);
        return lex_create(input_file_getc, input_file_topc, f, "file");
    }
    if (!(*repeat = isatty(STDIN_FILENO)))
    {
        f = fdopen(STDIN_FILENO, "r");
        return lex_create(input_file_getc, input_file_topc, f, "stdin");
    }
    if ((input = readline("42sh$ ")) == NULL || !strcmp(input->buf, "exit"))
    {
        string_free(input);
        *repeat = 0;
        return NULL;
    }
    return lex_create(input_string_getc, input_string_topc, input, "int");
}

void input_free(s_lexer *lexer, char *cmd, char *file)
{
    if (cmd)
    {
        s_string *str = lexer->input_state;
        string_free(str);
    }
    else if (file)
    {
        FILE *f = lexer->input_state;
        fclose(f);
    }
    sfree(lexer);
}

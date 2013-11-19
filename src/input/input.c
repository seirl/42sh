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

s_lexer *input_to_lexer(char *cmd, char *file, int *repeat)
{
    s_string *input;
    if (cmd)
    {
        input = string_create_from(cmd);
        return lex_create(input_string_getc, input_string_topc, input, "cmd");
    }
    if (file)
    {
        FILE *f = fopen(file, "r");
        return lex_create(input_file_getc, input_file_topc, f, "file");
    }
    *repeat = isatty(STDIN_FILENO);
    if (!*repeat)
    {
        //FIXME STDIN
        return NULL;
    }
    input = readline("42sh> ");
    if (input == NULL || !strcmp(input->buf, "exit"))
    {
        *repeat = 0;
        return NULL;
    }
    return lex_create(input_string_getc, input_string_topc, input, "int");
}

void input_free(s_lexer *lexer, char *cmd, char *file)
{
    s_string *str = lexer->input_state;
    FILE *f = lexer->input_state;
    if (cmd)
        string_free(str);
    else if (file)
        fclose(f);
}

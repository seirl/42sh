#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "string_utils.h"
#include "input.h"
#include "input_string.h"
#include "input_file.h"
#include "lexer_private.h"

s_lexer *input_to_lexer(char *cmd, char *file)
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
    return NULL;
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

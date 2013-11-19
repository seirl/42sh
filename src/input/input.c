#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "string_utils.h"
#include "input.h"
#include "input_string.h"

s_lexer *input_to_lexer(char *cmd)
{
    s_string *input;
    if (cmd)
    {
        input = string_create_from(cmd);
        return lex_create(input_string_getc, input_string_topc, input, "cmd");
    }
    return NULL;
}

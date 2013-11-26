#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"
#include "lexer.h"
#include "token.h"
#include "input_string.h"

void remove_special_backslash(s_string *str)
{
    for (int i = 0; str->buf[i]; ++i)
    {
        if (str->buf[i] == '\\')
        {
            char c = str->buf[i + 1];
            if (c == '$' || c == '`' || c == '\"' || c == '\n')
            {
                for (int j = 0; str->buf[i + j]; ++j)
                    str->buf[i + j] = str->buf[i + j + 1];
                str->len -= 1;
            }
        }
    }
}

#include <stdio.h>
void remove_backslash(s_string *str)
{
    printf("Remove back\n");
    for (int i = 0; str->buf[i]; ++i)
    {
        if (str->buf[i] == '\\')
        {
            char c = str->buf[i + 1];
            if (c != '\"')
            {
                for (int j = 0; str->buf[i + j]; ++j)
                    str->buf[i + j] = str->buf[i + j + 1];
                str->len -= 1;
            }
        }
    }
}

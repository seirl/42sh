#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "string_utils.h"
#include "env.h"

static void prompt_replace(s_string *prompt, size_t pos, size_t len, char *rep)
{
    char *tmp = strdup(prompt->buf + pos + 1 + len);
    string_del_from_end(prompt, prompt->len - pos);
    string_puts(prompt, rep);
    string_puts(prompt, tmp);
    free(tmp);
}

void prompt_expand(s_shell *shell, s_string *prompt)
{
    (void)shell;
    for (size_t i = 0; prompt->buf[i + 1]; i++)
    {
        if (prompt->buf[i] == '\\')
        {
#define X(Name, Len, Rep)                                           \
            if  (!strncmp(prompt->buf + i + 1, Name, Len))          \
            {                                                       \
                prompt_replace(prompt, i, Len, Rep);                \
                continue;                                           \
            }
#include "prompt.def"
#undef X
        }
    }
}

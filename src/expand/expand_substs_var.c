#include <stdlib.h>
#include <string.h>

#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"

s_string *expand_substs_var(const s_shell *shell, const s_string *word)
{
    if (word->buf[0] == '#')
    {
        const char *r = env_get(shell, word->buf + 1);
        return string_itoa(r ? strlen(r) : 0);
    }

    s_string *varname = string_create(0);
    for (unsigned i = 0; word->buf[i] != '\0' && word->buf[i] != ':'; i++)
        string_putc(varname, word->buf[i]);
    s_string *var_content = string_create_from(env_get(shell, varname->buf));
    free(varname);
    (void) var_content;

    return NULL;
}

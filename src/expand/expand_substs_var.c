#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"

s_string *expand_substs_var(const s_shell *shell, const s_string *word)
{
    (void) shell;
    (void) word;
    // TODO

    if (word->buf[0] == '#')
        return string_itoa(0);
    return NULL;
}

#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "shell_private.h"
#include "expand.h"
#include "env.h"

#define UNUSED(Var) Var += 1;

s_string *expand_squote(s_shell *shell, s_string *word)
{
    UNUSED(shell);
    return string_duplicate(word);
}

#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"

s_string *expand_squote(s_shell *shell, s_string *word)
{
    (void)shell;
    return string_duplicate(word);
}

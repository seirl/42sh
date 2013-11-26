#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"

s_string *expand_simple_var(s_shell *shell, s_string *word)
{
    return string_create_from(env_get(shell, word->buf));
}

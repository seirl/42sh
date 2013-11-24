#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"

s_string *expand_simple_var(const s_shell *shell, const s_string *word)
{
    return string_create_from(env_get(shell, word->buf));
}

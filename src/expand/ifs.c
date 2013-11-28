#include <stdio.h>
#include <stdlib.h>
#include "expand.h"
#include "ast.h"
#include "shell.h"
#include "env.h"
#include "smalloc.h"

#include "ifs.h"
#include "string_utils.h"

int is_ifs(s_shell *shell, char c)
{
    char *ifs = env_get(shell, "IFS");
    if (ifs == NULL)
        return 0;
    for (int i = 0; ifs[i]; ++i)
    {
        if (ifs[i] == c)
            return 1;
    }
    return 0;
}

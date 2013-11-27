#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "env_private.h"
#include "shell_private.h"
#include "hashtbl.h"
#include "env.h"
#include "smalloc.h"

static void add_cwd(s_shell *shell)
{
    char *curr_dir = scalloc(4096, sizeof (char));
    curr_dir = getcwd(curr_dir, 4096);
    env_set(shell, curr_dir, "PWD");
    sfree(curr_dir);
}

void env_default_var(s_shell *shell)
{
    add_cwd(shell);
}

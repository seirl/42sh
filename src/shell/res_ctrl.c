#include <stdio.h>
#include <stdlib.h>

#include "res_ctrl.h"
#include "shell_eval.h"

static int load_rc(s_shell *shell, const char *file)
{
    int ret;
    FILE *f;
    f = fopen(file, "r");
    if (f)
    {
        if (!(ret = shell_eval_file(shell, f, file)))
            return ret;
    }
    return 0;
}

int rc_file_load(s_shell *shell)
{
    int ret;

    if (!(ret = load_rc(shell, "/etc/42shrc")))
        return ret;

    char *home = getenv("HOME");
    if (home)
    {
        char path[255];
        sprintf(path, "%s/.42shrc", home);
        if (!(ret = load_rc(shell, path)))
            return ret;
    }
    return 0;
}

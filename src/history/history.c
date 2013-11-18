#include <stdio.h>
#include "env.h"
#include "history.h"

static int open_history_file(void)
{
    if (!env_get("HISTFILE"))
        env_set("HISTFILE", "~/.sh_history");

    FILE *histfile = fopen(env_get("HISTFILE"), "a+");
}

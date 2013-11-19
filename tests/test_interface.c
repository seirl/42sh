#include <stdlib.h>
#include <stdio.h>
#include "options.h"

int main(int argc, char *argv[])
{
    char *cmd;
    char *file;
    int ret = parse_options(argc, argv, &cmd, &file);
    if (ret == 2)
        return ret;

    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>
#include "options.h"

int main(int argc, char *argv[])
{
    char *cmd;
    int ret = parse_options(argc, argv, &cmd);
    if (ret)
        return ret;

    return EXIT_SUCCESS;
}

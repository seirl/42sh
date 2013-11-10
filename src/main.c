#include <stdlib.h>
#include "options.h"

int main(int argc, char *argv[])
{
    int ret = parse_options(argc, argv);
    if (ret)
        return ret;
    return EXIT_SUCCESS;
}

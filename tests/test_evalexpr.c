#include <stdio.h>
#include <stdlib.h>

#include "evalexpr.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fputs("usage: test_evalexpr <EXPR>\n", stderr);
        return 1;
    }

    printf("%ld\n", evalexpr(argv[1]));
    return 0;
}

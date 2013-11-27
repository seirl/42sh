#include <stdio.h>
#include <stdlib.h>

#include "match.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fputs("usage: test_fnmatch <PATTERN> <INPUT>\n", stderr);
        return 1;
    }

    printf("%d\n", my_fnmatch(argv[1], argv[2]));
    return 0;
}

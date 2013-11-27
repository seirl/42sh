#include <stdio.h>
#include <stdlib.h>

#include "match.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fputs("usage: test_glob <PATTERN>\n", stderr);
        return 1;
    }

    s_globr *g = my_glob(argv[1], 0);
    for (unsigned i = 0; i < g->count; i++)
        printf("%s\n", g->paths[i]);
    my_globfree(g);
    return 0;
}

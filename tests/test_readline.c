#include <stdio.h>
#include "string_utils.h"
#include "readline.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    readline();
#if 0
    if (argc != 2)
        return 1;

    s_string *line = readline(argv[1]);
    while (line)
    {
        printf("\nRead: [%s]\n", line->buf);
        string_free(line);
        line = readline(argv[1]);
    }
#endif

    return 0;
}
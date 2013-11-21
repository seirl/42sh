#include <stdio.h>
#include "string_utils.h"
#include "readline.h"
#include "env.h"

int main(void)
{
    s_string *line;
    while ((line = readline("test$ ")))
    {
        printf("Read: [%s]\n", line->buf);
        string_free(line);
    }

    env_free();
    return 0;
}

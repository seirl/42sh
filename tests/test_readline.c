#include <stdio.h>
#include "string_utils.h"
#include "readline.h"
#include "env.h"

int main(void)
{
    s_string *line = readline("test$ ");
    while (1)
    {
        if (string_equal(line, "exit"))
            break;
        printf("Read: [%s]\n", line->buf);
        string_free(line);

        line = readline("test$ ");
    }

    string_free(line);
    env_free();
    return 0;
}

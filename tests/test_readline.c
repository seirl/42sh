#include <stdio.h>
#include "string_utils.h"
#include "readline.h"
#include "env.h"

int main(void)
{
    s_string *line = readline();
    while (1)
    {
        printf("\nRead: [%s]\n", line->buf);
        string_free(line);

        line = readline();
        if (string_equal(line, "exit"))
            break;
    }

    string_free(line);
    env_free();
    return 0;
}

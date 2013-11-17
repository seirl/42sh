#include <stdio.h>
#include "string_utils.h"
#include "readline.h"
#include "env.h"

int main(void)
{
    s_string *line = readline();
    while (1)
    {
        if (string_equal(line, "exit"))
            break;
        printf("\nRead: [%s]\n", line->buf);
        string_free(line);

        line = readline();
    }

    string_free(line);
    env_free();
    return 0;
}

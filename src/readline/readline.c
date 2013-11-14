#include <unistd.h>
#include <stdio.h>
#include "string_utils.h"
#include "characters.h"
#include "terminal.h"

static void handle_special(s_string *buffer, char c) // TODO: test
{
    // TODO
    (void)buffer;
    (void)c;
}

s_string *readline(const char *prompt) // TODO: test
{
    setup_terminal();
    printf("%s", prompt);
    fflush(stdout);

    char c;
    int index = 0;
    s_string *buffer = string_create(10);
    while ((c = read(STDIN_FILENO, &c, 1)) > 0)
    {
        if (c == '\n' && buffer->buf[index - 1] != '\\')
            break;
        else if (readline_printable(c))
        {
            string_putc(buffer, c);
            index++;
        }
        else
            handle_special(buffer, c);
    }

    reset_terminal();

    string_replace(buffer, "\\\n", ""); // TODO: string_replace not implemented.
    return buffer;
}

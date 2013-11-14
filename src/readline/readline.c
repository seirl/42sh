#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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
    atexit(reset_terminal);

    char c;
    int read_ret;
    int index = 0;
    s_string *buffer = string_create(10);
    while ((read_ret = read(STDIN_FILENO, &c, 1)) > 0)
    {
        write(STDOUT_FILENO, &c, 1);
        if (c == '\n' && buffer->buf[index - 1] != '\\')
            break;
        else if (readline_printable(c))
        {
            string_putc(buffer, c);
            index++;
        }
        else if (c == '')
            handle_special(buffer, c);
    }

    reset_terminal();

    if (!read_ret && !index) // TODO: EOF is not handled well
        return NULL;

    s_string *rep_buffer = string_replace(buffer, "\\\n", ""); // TODO: string_replace not implemented.
    string_free(buffer);
    return rep_buffer;
}

#include <unistd.h>
#include <stdio.h>
#include "string_utils.h"
#include "characters.h"

static void handle_special(s_string *buffer, char c) // TODO: test
{
    // TODO
    (void)buffer;
    (void)c;
}

s_string *readline(const char *prompt) // TODO: test
{
    printf("%s ", prompt); // TODO: should this whitespace be there ?
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

    string_replace(buffer, "\\\n", ""); // TODO: string_replace not implemented.
    return buffer;
}

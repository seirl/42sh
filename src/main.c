#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc < 1)
        return 1;
    s_string *input = string_create(0);
    for (int i = 1; i < argc; ++i)
    {
        string_puts(input, argv[i]);
        string_putc(input, ' ');
    }
    //s_token_queue *q = lex(input->buf);
    printf("%s\n", input->buf);
    string_free(input);
    return EXIT_SUCCESS;
}

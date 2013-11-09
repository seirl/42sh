#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "sighandler.h"
#include "smalloc.h"

int main(int argc, char *argv[])
{
    signal_init();
    if (argc < 1)
        return 1;
    s_string *input = string_create(0);
    for (int i = 1; i < argc; ++i)
    {
        string_puts(input, argv[i]);
        string_putc(input, ' ');
    }
    s_token_queue *q = lex(input->buf);
    s_token *tok = NULL;
    while ((tok = token_dequeue(q))->type != T_EOF)
    {
        token_print(tok);
        token_free(tok);
    }
    token_free(tok);
    string_free(input);
    token_queue_free(q);

    smalloc_clean();
    return EXIT_SUCCESS;
}

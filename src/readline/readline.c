#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"
#include "terminal.h"
#include "key.h"
#include "env.h"

#include "wrapper.h"
s_string *readline()
{
    s_term *term = term_get();
    if (term == NULL)
        return NULL;
    char c;

    if (!env_get("PS1"))
        env_set("42sh$ ", "PS1");
    printf("%s", env_get("PS1"));
    fflush(stdout);
    e_next_action ret;
    while (1)
    {
        ret = getkey(term, &c);
        if (ret == CONTINUE && !isprint(c))
            continue;
        if (ret == RETURN || ret == ERROR)
            break;
        string_putc(term->input, c);
        printf("%c", c);
        fflush(stdout);
    }

    s_string *input = (ret == ERROR) ? NULL : string_duplicate(term->input);
    term_close();
    fflush(stdout);
    return input;
}

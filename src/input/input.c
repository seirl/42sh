#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include "string_utils.h"
#include "input.h"
#include "readline.h"
#include "env.h"
#include "lexer_private.h"
#include "lexer.h"

static s_input_state input =
{
    NULL,
    0,
};

static s_string *generic_getline(void *lexer_state)
{
    if (isatty(STDIN_FILENO))
    {
        if (!env_get("PS1"))
            env_set("42sh$ ", "PS1");
        if (!env_get("PS2"))
            env_set("> ", "PS1");

        s_lexer *lexer = lexer_state;
        return readline(env_get(lexer->quoted ? "PS2" : "PS1"));
    }
    char *line = NULL;
    getline(&line, 0, stdin);
    return string_create_from(line);
}

static char generic_getc(void *lexer_state, int incr)
{
    if (input.line && input.index >= input.line->len)
    {
        string_free(input.line);
        input.line = NULL;
    }
    if (!input.line)
    {
        input.line = generic_getline(lexer_state);
        input.index = 0;
    }

    return input.line->buf[input.index + incr];
}

char input_getc(void *lexer_state)
{
    return generic_getc(lexer_state, 1);
}

char input_topc(void *lexer_state)
{
    return generic_getc(lexer_state, 0);
}

#include <stdio.h>
#include <stdlib.h>

#include "input_string.h"
#include "lexer.h"
#include "smalloc.h"
#include "string_utils.h"
#include "token.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fputs("usage: test_lexer <INPUT> [(W|T|H|S)* [HERE-DOC]*]\n", stderr);
        return 1;
    }

    s_input *input;
    s_token *tok;
    s_lexer *lexer;
    e_token_type t = T_WORD;

    input = input_string_create(string_create_from(argv[1]), "<INPUT>");
    lexer = lex_create(NULL, input, LEX_ALL);
    if (argc == 2)
    {
        do {
            tok = lex_token(lexer);
            token_print(tok);
            t = tok->type;
            token_free(tok);
        } while (t != T_EOF);
    }
    else
    {
        size_t heredoc_idx = 3;
        s_string *delim;
        for (; *argv[2] != '\0'; argv[2]++)
        {
            switch (*argv[2])
            {
            case 'T':
                tok = lex_token(lexer);
                break;
            case 'W':
                tok = lex_word(lexer);
                break;
            case 'H':
                delim = string_create_from(argv[heredoc_idx++]);
                tok = lex_heredoc(lexer, delim);
                string_free(delim);
                break;
            case 'S':
                delim = string_create_from(argv[heredoc_idx++]);
                tok = lex_heredoc_strip(lexer, delim);
                string_free(delim);
                break;
            default:
                fputs("BAD TOKEN OPERATION\n", stderr);
                return 1;
            }
            token_print(tok);
            t = tok->type;
            token_free(tok);
        }
    }

    lex_delete(lexer);
    sfree(lexer);
    smalloc_clean();
}

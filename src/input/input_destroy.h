#include "input_private.h"

void input_destroy(s_lexer *lexer, char *cmd, char *file)
{
    if (cmd)
    {
        s_string *str = lexer->input_state;
        string_free(str);
    }
    else if (file)
    {
        FILE *f = lexer->input_state;
        fclose(f);
    }
    sfree(lexer);
}

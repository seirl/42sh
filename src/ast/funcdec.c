#include "ast.h"
#include "smalloc.h"

s_ast_funcdec *ast_func_dec_new(void)
{
    s_ast_funcdec *fd = smalloc(sizeof (s_ast_funcdec));

    fd->name = NULL;
    fd->shell_cmd = NULL;

    return fd;
}

void ast_func_dec_delete(s_ast_funcdec *fd)
{
    if (!fd)
        return;

    ast_shell_cmd_delete(fd->shell_cmd);
    string_free(fd->name);

    sfree(fd);
}

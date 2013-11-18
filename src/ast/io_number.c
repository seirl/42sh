#include "ast.h"
#include "smalloc.h"

s_ast_io_number *ast_io_number_new(void)
{
    s_ast_io_number *io = smalloc(sizeof (s_ast_io_number));

    io->io_number = 0;

    return io;
}

void ast_io_number_delete(s_ast_io_number *io)
{
    if (!io)
        return;

    sfree(io);
}

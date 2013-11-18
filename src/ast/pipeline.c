#include "ast.h"
#include "smalloc.h"

s_ast_pipeline *ast_pipeline_new(void)
{
    s_ast_pipeline *pipeline = smalloc(sizeof (s_ast_pipeline));

    pipeline->inverted = 0;
    pipeline->cmd = NULL;
    pipeline->next = NULL;

    return pipeline;
}

void ast_pipeline_delete(s_ast_pipeline *pipeline)
{
    if (!pipeline)
        return;

    ast_pipeline_delete(pipeline->next);
    ast_cmd_delete(pipeline->cmd);

    sfree(pipeline);
}

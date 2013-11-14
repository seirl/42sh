#include "exec.h"

void exec_redir_node(struct redir_node *node)
{
    /* Expansion here */
#if 0
    switch (node->type)
    {
        case L_TO_R_S:
            exec_ltors(node);
            break;
        case R_TO_L_S:
            exec_rtols(node);
            break;
        case L_TO_R_D:
            exec_ltord(node);
            break;
        case R_TO_L_D:
            exec_rtold(node);
            break;
        case L_TO_R_SA:
            exec_ltorsa(node);
            break;
        case R_TO_L_SA:
            exec_rtolsa(node);
            break;
        case L_TO_R_SO:
            exec_ltorso(node);
            break;
        case R_TO_L_DD:
            exec_rtoldd(node);
            break;
        case LR_TO_RL:
            exec_bidirect(node);
            break;
    }
#endif
    node = node - 0;
    assert(0);
}

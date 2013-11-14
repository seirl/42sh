#include "exec.h"

int main(void)
{
/*======================================================================*/
/*======================================================================*/
/*======================================================================*/
            /* if command:  if      grep nass /etc/passwd   */
            /*              then    touch nass              */
            /*              else    ls -lsa                 */
    char *grep_cmd = "grep";
    char *grep_arg = "nass";
    char *grep_arg2 = "/etc/passwd";
    char **grep_cmd_str = malloc(sizeof (char *) * 4);
    grep_cmd_str[0] = grep_cmd;
    grep_cmd_str[1] = grep_arg;
    grep_cmd_str[2] = grep_arg2;
    grep_cmd_str[3] = NULL;
    struct cmd_node grep_command_node = {
        .argv = grep_cmd_str,
        .prefix = NULL
    };
    union ast_node_child cond_child;
    cond_child.cmd_n = grep_command_node;
    struct ast_node cond_node = {
        .next = cond_child,
        .type = CMD
    };

    char *touch_cmd = "touch";
    char *touch_arg = "nass";
    char **touch_cmd_str = malloc(sizeof (char *) * 3);
    touch_cmd_str[0] = touch_cmd;
    touch_cmd_str[1] = touch_arg;
    touch_cmd_str[2] = NULL;
    struct cmd_node touch_command_node = {
        .argv = touch_cmd_str,
        .prefix = NULL
    };
    union ast_node_child true_child;
    true_child.cmd_n = touch_command_node;
    struct ast_node true_node = {
        .next = true_child,
        .type = CMD
    };

    char *ls_cmd = "ls";
    char *ls_arg = "-lsa";
    char **ls_cmd_str = malloc(sizeof (char *) * 3);
    ls_cmd_str[0] = ls_cmd;
    ls_cmd_str[1] = ls_arg;
    ls_cmd_str[2] = NULL;
    struct cmd_node ls_command_node = {
        .argv = ls_cmd_str,
        .prefix = NULL
    };
    union ast_node_child false_child;
    false_child.cmd_n = ls_command_node;
    struct ast_node false_node = {
        .next = false_child,
        .type = CMD
    };

    struct if_node if_node = {
        .cond = &cond_node,
        .if_true = &true_node,
        .if_false = &false_node
    };
    union ast_node_child if_child;
    if_child.if_n = if_node;
    struct ast_node main_if_node;
    main_if_node.next = if_child;
    main_if_node.type = IF;

    exec_node(&main_if_node);
    return 0;
}

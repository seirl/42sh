#include "exec.h"

void exec_prefixes(s_ast_prefix *prefix)
{
    while (prefix)
    {
        if (prefix->redirection)
            exec_redirection(prefix->redirection);
        if (prefix->assignment)
            exec_assignment(prefix->assignment);
        prefix = prefix->next;
    }
}

int element_list_len(s_ast_element *elt)
{
    int count = 0;

    while (elt)
    {
        if (elt->word)
            count += 1;
        elt = elt->next;
    }
    return count;
}

char **elements_to_argv(s_ast_element *element, int len)
{
    char **cmd_argv = smalloc(sizeof (char *) * (len + 1));
    s_string *str = NULL;

    for (int i = 0; i < len; ++i)
    {
        if (!element->word)
        {
            element = element->next;
        }
        else
        {
            if ((str = expand_compound(element->word)) == NULL)
                return NULL;
            cmd_argv[i] = smalloc(sizeof (char) * (str->len + 1));
            memcpy(cmd_argv[i], str->buf, str->len);
            cmd_argv[i][str->len] = '\0';
        }
        element = element->next;
    }
    cmd_argv[len] = NULL;
    return cmd_argv;
}

void exec_elements_redir(s_ast_element *elt)
{
    while (elt)
    {
        if (elt->redirection)
            exec_redirection(elt->redirection);
        elt = elt->next;
    }
}

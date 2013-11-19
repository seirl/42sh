#include "exec.h"

void exec_prefixes(s_ast_prefix *prefix)
{
    while (prefix)
    {
        if (prefix->redirection)
        {
            s_redir_context *cont = exec_redirection(prefix->redirection);
            if (prefix->assignment)
                exec_assignment(prefix->assignment);
            restore_redir_context(cont);
        }
        else if (prefix->assignment)
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

static int element_redir_list_len(s_ast_element *elt)
{
    int count = 0;

    while (elt)
    {
        if (elt->redirection)
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

s_redir_context **exec_elements_redir(s_ast_element *elt)
{
    int len = element_redir_list_len(elt);
    s_redir_context **contexts =
        smalloc(sizeof (s_redir_context *) * (len + 1));
    int count = 0;
    while (elt)
    {
        if (elt->redirection)
        {
            s_redir_context *cont = exec_redirection(elt->redirection);
            contexts[count] = cont;
            count += 1;
        }
        elt = elt->next;
    }
    contexts[len] = NULL;
    return contexts;
}

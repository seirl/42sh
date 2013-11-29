#include "exec.h"
#include "expand.h"
#include "smalloc.h"

void exec_prefixes(s_shell *shell, s_ast_prefix *prefix)
{
    while (prefix)
    {
        if (prefix->redirection)
        {
            s_redir_context *cont = exec_redirection(shell,
                                                     prefix->redirection);
            if (prefix->assignment)
                exec_assignment(shell, prefix->assignment);
            restore_redir_context(cont);
        }
        else if (prefix->assignment)
            exec_assignment(shell, prefix->assignment);
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

char **elements_to_argv(s_shell *shell, s_ast_element *element, int *len)
{
    expand_element(shell, element);
    *len = element_list_len(element);
    char **cmd_argv = smalloc(sizeof (char *) * (*len + 1));
    s_string *str = NULL;

    for (int i = 0; i < *len; ++i)
    {
        if (!element->word)
        {
            element = element->next;
        }
        else
        {
            if ((str = expand_compound(shell, element->word)) == NULL)
                return NULL;
            cmd_argv[i] = smalloc(sizeof (char) * (str->len + 1));
            memcpy(cmd_argv[i], str->buf, str->len);
            cmd_argv[i][str->len] = '\0';
            string_free(str);
        }
        element = element->next;
    }
    cmd_argv[*len] = NULL;
    return cmd_argv;
}

s_redir_context **exec_elements_redir(s_shell *shell, s_ast_element *elt)
{
    //expand_element(shell, elt);
    int len = element_redir_list_len(elt);
    s_redir_context **contexts =
        smalloc(sizeof (s_redir_context *) * (len + 1));
    s_ast_redirection_list **elt_redirs =
        smalloc(sizeof (s_ast_redirection_list *) * (len + 1));
    int count = 0;

    while (elt)
    {
        if (elt->redirection)
            elt_redirs[count++] = elt->redirection;
        elt = elt->next;
    }
    elt_redirs[len] = NULL;
    for (int i = 1; i <= count; ++i)
    {
            s_redir_context *cont =
                exec_redirection(shell, elt_redirs[len - i]);
            contexts[i - 1] = cont;
    }
    contexts[len] = NULL;
    sfree(elt_redirs);
    return contexts;
}

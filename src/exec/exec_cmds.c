#include "exec.h"

s_string *expand_word(s_ast_word *word)
{
    if (word)
        return word->str;
    return NULL;
}

s_string *expand_compound(s_ast_compound_word *word)
{
    if (word && word->word)
        return word->word->str;
    return NULL;
}

void exec_assignment(s_ast_assignment *assign)
{
    s_string *value = expand_compound(assign->value);
    vars_set(assign->name, value);
}

handler builtin_handler(char *name)
{
    if (!shell.builtins || !shell.built_count)
        return NULL;
    for (unsigned int i = 0; i < shell.built_count; ++i)
        if (!strcmp(shell.builtins[i].name, name))
            return shell.builtins[i].callback;
    return NULL;
}

int redir_list_len(s_ast_redirection_list *redir)
{
    int count = 0;
    while (redir)
    {
        count += 1;
        redir = redir->next;
    }
    return count;
}

s_redir_context *save_redir_context(s_ast_redirection_list *redir)
{
    int size = redir_list_len(redir);
    s_redir_context *context = smalloc(sizeof (s_redir_context) * (size + 1));

    for (int i = 0; i < size; ++i)
    {
        context[i].old_fd = redir->io->io_number;
        context[i].tmp_fd = dup(redir->io->io_number);
        redir = redir->next;
    }
    context[size].old_fd = -1;
    return context;
}

int word_to_fd(s_string *str)
{
    long fd = 0;
    char *endptr = NULL;

    if (str->len == 0)
        return -1;
    if ((string_index(str, 0) == '-') && (string_index(str, 1) == '\0'))
        return -2;
    fd = strtol(str->buf, &endptr, 10);
    if (endptr == str->buf || endptr != 0)
        return -1;

    return fd;
}

int set_redir(s_ast_redirection_list *redir)
{
    int fd = 0;
    int index = 0;

    while (redir)
    {
        if (redir->type == REDIR_WRITE)                 /** >   */
        {
            s_string *filename = expand_compound(redir->word);
            if ((fd = open(filename->buf,
                                  O_CREAT | O_WRONLY | O_TRUNC, 666)) == -1)
                fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
            dup2(fd, redir->io->io_number);
            close(fd);
        }
        else if (redir->type == REDIR_WRITE_UPDATE)     /** >>  */
        {
            s_string *filename = expand_compound(redir->word);
            if ((fd = open(filename->buf,
                           O_CREAT | O_WRONLY | O_APPEND, 666)) == -1)
                fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
            dup2(fd, redir->io->io_number);
            close(fd);
        }
        else if (redir->type == REDIR_READ)             /** <   */
        {
            s_string *filename = expand_compound(redir->word);
            if ((fd = open(filename->buf, O_RDONLY)) == -1)
                fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
            dup2(fd, redir->io->io_number);
            close(fd);
        }
        else if (redir->type == REDIR_HEREDOC)          /** <<  */
            write(redir->io->io_number,
                  redir->heredoc->heredoc->buf,
                  redir->heredoc->heredoc->len);
        else if (redir->type == REDIR_HEREDOC_STRIP)    /** <<- */
            write(redir->io->io_number,
                  redir->heredoc->heredoc->buf,
                  redir->heredoc->heredoc->len);
        else if (redir->type == REDIR_DUPLICATE_INPUT)  /** <&  */
        {
            s_string *filename = expand_compound(redir->word);
            fd = word_to_fd(filename);
            if (fd == -1)
                fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
            else if (fd == -2)
                close(fd); /* FIXME error handling */
            dup2(fd, redir->io->io_number);
        }
        else if (redir->type == REDIR_DUPLICATE_OUTPUT) /** >&  */
        {
            s_string *filename = expand_compound(redir->word);
            fd = word_to_fd(filename);
            if (fd == -1)
                fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
            else if (fd == -2)
                close(fd); /* FIXME error handling */
            dup2(fd, redir->io->io_number);
        }
        else if (redir->type == REDIR_CLOBBER)          /** >|  */
        {
            s_string *filename = expand_compound(redir->word);
            if ((fd = open(filename->buf,
                           O_CREAT | O_WRONLY | O_TRUNC, 666)) == -1)
                fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
            dup2(fd, redir->io->io_number);
            close(fd);
        }
        else if (redir->type == REDIR_READ_WRITE)       /** <>  */
        {
            s_string *filename = expand_compound(redir->word);
            if ((fd = open(filename->buf, O_CREAT | O_RDWR | O_TRUNC)) == -1)
                fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
            dup2(fd, redir->io->io_number);
            close(fd);
        }
        redir = redir->next;
        index += 1;
    }
    return 0;
}

void restore_redir_context(s_redir_context *context)
{
    for (int i = 0; context[i].old_fd != -1; ++i)
        dup2(context[i].tmp_fd, context[i].old_fd);
    sfree(context);
}

void exec_redirection(s_ast_redirection_list *redir)
{
    s_redir_context *context;
    context = save_redir_context(redir);
    shell.status = set_redir(redir);
    restore_redir_context(context);
}

int compound_word_len(s_ast_compound_word *word)
{
    int count = 0;

    while (word)
    {
        count += 1;
        word = word->next;
    }
    return count;
}

char **compword_to_argv(s_ast_compound_word *word, int len)
{
    char **cmd_argv = smalloc(sizeof (char *) * (len + 1));
    s_string *str = NULL;

    for (int i = 0; i < len; ++i)
    {
        str = expand_word(word->word);
        cmd_argv[i] = smalloc(sizeof (char) * str->len);
        memcpy(cmd_argv[i], str->buf, str->len);
        word = word->next;
    }
    cmd_argv[len] = NULL;
    return cmd_argv;
}

void exec_argv(char **argv)
{
    assert(argv && argv[0]);
    execvp(argv[0], argv);
    if (errno == ENOENT)
    {
        if (strchr(argv[0], '/') == NULL)
            fprintf(stderr, "%s: command not found.\n", argv[0]);
        else
            fprintf(stderr, "%s: no such file or directory.\n", argv[0]);
    }
    exit((errno == ENOENT) ? 127 : 126);
}

int exec_program(char **cmd_argv, s_ast_prefix *prefixes)
{
    pid_t pid;
    int st;

    if ((pid = fork()) == -1)
    {
        fprintf(stderr, "Cannot fork.\n");
        return -1;
    }
    if (pid == 0)
    {
        exec_prefixes(prefixes);
        exec_argv(cmd_argv);
        fprintf(stderr, "Execution flow corrupted.\n");
        assert(0);
        return 1;
    }
    waitpid(pid, &st, 0);
    return st;
}

void exec_cmd_word(s_ast_compound_word *word)
{
    int len = compound_word_len(word);
    handler callback = NULL;
    char **cmd_argv = compword_to_argv(word, len);

    s_ast_shell_cmd *func_body = funcs_get(cmd_argv[0]);
    if (func_body)
        exec_shell_cmd(func_body);
    else
    {
        if ((callback = builtin_handler(cmd_argv[0])) != NULL)
        {
            shell.status = callback(cmd_argv);
        }
        else
            shell.status = exec_program(cmd_argv, NULL);
    }
    for (int i = 0; i < len; ++i)
        sfree(cmd_argv[i]);
    sfree(cmd_argv);
}

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

void exec_simple_cmd(s_ast_simple_cmd *cmd)
{
    int len = element_list_len(cmd->elements);
    handler callback = NULL;
    exec_elements_redir(cmd->elements);
    char **cmd_argv = elements_to_argv(cmd->elements, len);

    s_ast_shell_cmd *func_body = funcs_get(cmd_argv[0]);
    if (func_body)
    {
        exec_prefixes(cmd->prefixes);
        exec_shell_cmd(func_body);
    }
    else
    {
        if ((callback = builtin_handler(cmd_argv[0])) != NULL)
        {
            exec_prefixes(cmd->prefixes);
            shell.status = callback(cmd_argv);
        }
        else
            shell.status = exec_program(cmd_argv, cmd->prefixes);
    }
    /*
    for (int i = 0; i < len; ++i)
        sfree(cmd_argv[i]);
    sfree(cmd_argv);
    */
}

void exec_else(s_ast_else *else_cmd)
{
    exec_ast_list(else_cmd->elif_predicate);
    if (!shell.status)
        exec_ast_list(else_cmd->elif_cmds);
    else
        exec_ast_list(else_cmd->else_cmds);
}

void exec_if(s_ast_if *if_cmd)
{
    exec_ast_list(if_cmd->predicate);
    if (!shell.status)
        exec_ast_list(if_cmd->then_cmds);
    /* FIXME: elif */
    else
        if (if_cmd->else_clause)
            exec_else(if_cmd->else_clause);
}

void exec_while(s_ast_while *while_cmd)
{
    for (exec_ast_list(while_cmd->predicate);
         !shell.status;
         exec_ast_list(while_cmd->predicate))
    {
        exec_ast_list(while_cmd->cmds);
    }
}

void exec_until(s_ast_until *until_cmd)
{
    for (exec_ast_list(until_cmd->predicate);
         shell.status;
         exec_ast_list(until_cmd->predicate))
    {
        exec_ast_list(until_cmd->cmds);
    }
}

void exec_for(s_ast_for *for_cmd)
{
    s_string *value = NULL;
    while (for_cmd->values
           && (value = expand_compound(for_cmd->values->word)))
    {
        vars_set(expand_word(for_cmd->identifier), value);
        exec_ast_list(for_cmd->cmd_list);
        for_cmd->values = for_cmd->values->next;
    }
}

void exec_subshell_cmd(s_ast_shell_cmd *shell_cmd)
{
    if (!shell_cmd)
    {
        fprintf(stderr, "Invalid SUBSHELL command.\n");
        return;
    }
    exec_shell_cmd(shell_cmd);
    fprintf(stderr, "SUBSHELL commands not implemented yet.\n");
    return;
}

void exec_case(s_ast_case *node)
{
    if (!node)
    {
        fprintf(stderr, "Invalid CASE command.\n");
        return;
    }
    fprintf(stderr, "CASE commands not implemented yet.\n");
    return;
}

void exec_shell_cmd(s_ast_shell_cmd *shell_cmd)
{
    if (shell_cmd->cmd_list)
    {
        if (shell_cmd->subshell == 1)
            exec_subshell_cmd(shell_cmd);
        else
            exec_ast_list(shell_cmd->cmd_list);
    }
    else
    {
        if (shell_cmd->ctrl_structure == AST_IF)
            exec_if(shell_cmd->ctrl.ast_if);
        else if (shell_cmd->ctrl_structure == AST_FOR)
            exec_for(shell_cmd->ctrl.ast_for);
        else if (shell_cmd->ctrl_structure == AST_WHILE)
            exec_while(shell_cmd->ctrl.ast_while);
        else if (shell_cmd->ctrl_structure == AST_UNTIL)
            exec_until(shell_cmd->ctrl.ast_until);
        else if (shell_cmd->ctrl_structure == AST_CASE)
            exec_case(shell_cmd->ctrl.ast_case);
    }
}

void exec_shell_cmd_node(s_ast_shell_cmd *shell_cmd,
                         s_ast_redirection_list *redir)
{
    exec_redirection(redir);
    exec_shell_cmd(shell_cmd);
}

void exec_func_dec(s_ast_funcdec *funcdec)
{
    funcs_set(funcdec->name->buf, funcdec->shell_cmd);
}

void exec_funcdec_node(s_ast_funcdec *funcdec,
                       s_ast_redirection_list *redir)
{
    exec_redirection(redir);
    exec_func_dec(funcdec);
}

void exec_cmd_node(s_ast_cmd *node)
{
    if (node->simple_cmd)
        exec_simple_cmd(node->simple_cmd);
    else if (node->shell_cmd)
        exec_shell_cmd_node(node->shell_cmd, node->redirections);
    else if (node->func_dec)
        exec_funcdec_node(node->func_dec, node->redirections);
    else
        fprintf(stderr, "unkown command type.\n");
}

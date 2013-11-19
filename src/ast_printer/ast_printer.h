#ifndef AST_PRINTER_H
# define AST_PRINTER_H

s_string *clean(s_string *s);
unsigned long int ph(void *ptr);
void print_pipeline(s_ast_pipeline *node, void *prev, FILE *out);
void print_cmd_list(s_ast_list *n, void *prev, FILE *out);
void print_ast(s_ast_input *in, FILE *out);

void print_word(s_ast_word *w, void *prev, FILE *out);
void print_compound_word(s_ast_compound_word *w, void *prev, FILE *out);
void print_word_list(s_ast_word_list *w, void *prev, FILE *out);
void print_heredoc(s_ast_heredoc *w, void *prev, FILE *out);

void print_assignment(s_ast_assignment *n, void *prev, FILE *out);
void print_redirection_list(s_ast_redirection_list *n, void *prev, FILE *out);
void print_prefix(s_ast_prefix *n, void *prev, FILE *out);
void print_element(s_ast_element *n, void *prev, FILE *out);

void print_ast_case(s_ast_case *n, void *prev, FILE *out);
void print_ast_for(s_ast_for *n, void *prev, FILE *out);
void print_ast_if(s_ast_if *n, void *prev, FILE *out);
void print_ast_while(s_ast_while *n, void *prev, FILE *out);
void print_ast_until(s_ast_until *n, void *prev, FILE *out);

void print_simple_cmd(s_ast_simple_cmd *n, void *prev, FILE *out);
void print_shell_cmd(s_ast_shell_cmd *n, void *prev, FILE *out);
void print_func_dec(s_ast_funcdec *n, void *prev, FILE *out);
void print_cmd(s_ast_cmd *c, void *prev, FILE *out);

#endif /* !AST_PRINTER_H */

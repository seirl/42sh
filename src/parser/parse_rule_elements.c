#include "parser_private.h"
#include "string_utils.h"

s_ast_element *parse_rule_elements(s_parser *parser)
{
    s_ast_compound_word *word = NULL;
    s_ast_redirection_list *redirection = NULL;

    redirection = parse_rule_redirection(parser);
    word = parse_compound_word(parser);

    if (!word && !redirection)
        return NULL;

    s_ast_element *element = ast_element_new();

    element->word = word;
    element->redirection = redirection;
    element->next = parse_rule_elements(parser);

    return element;
}

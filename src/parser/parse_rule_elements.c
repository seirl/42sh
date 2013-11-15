#include "parser_private.h"
#include "smalloc.h"
#include "string_utils.h"

static s_ast_element *element_new(void)
{
    s_ast_element *element = smalloc(sizeof (s_ast_element));

    element->next = NULL;
    element->word = NULL;
    element->redirection = NULL;
    
    return element;
}

s_ast_element *parse_rule_elements(s_parser *parser)
{
    s_ast_word *word = NULL;
    s_ast_redirection *redirection = NULL;
    s_token *tok;

    // FIXME: make_parse_word
    tok = lex_look_token(parser->lexer);
    if (tok->type == T_WORD)
    {
        word = smalloc(sizeof (s_ast_word));
        word->word = string_duplicate(tok->value.str);
    }
    token_free(tok);

    // TODO: redirection = parse_rule_redirection(word);

    if (!word && !redirection)
        return NULL;

    s_ast_element *element = element_new();
    element->next = parse_rule_elements(parser);
    return element;
}

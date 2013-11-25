#ifndef INPUT_STRING_H
# define INPUT_STRING_H

# include "input.h"

# include "string_utils.h"

/** @brief Create a new input from a string. */
s_input *input_string_create(s_string *str, const char *source);
/** @brief Delete a string input. */
void input_string_delete(s_input *input);
/** @brief Eat a char from a string (input_state = s_string). */
char input_string_getc(s_input *input);
/** @brief Get a char from a string (input_state = s_string). */
char input_string_topc(s_input *input);
/** @brief Not used by string input. */
int input_string_next(s_input *input, void *param);

#endif /* !INPUT_STRING_H */

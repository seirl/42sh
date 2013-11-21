#ifndef INPUT_STRING_H
# define INPUT_STRING_H

# include "input.h"

# include "string_utils.h"

/** @brief Create a new input from a string. */
s_input *input_string_create(s_string *str, const char *source);
/** @brief Eat a char from a string (input_state = s_string). */
char input_string_getc(s_input *input);
/** @brief Get a char from a string (input_state = s_string). */
char input_string_topc(s_input *input);

#endif /* !INPUT_STRING_H */

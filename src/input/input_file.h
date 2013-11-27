#ifndef INPUT_FILE_H
# define INPUT_FILE_H

# include <stdio.h>

# include "input_private.h"

/** @brief Create a file input from a FILE * (gets ownership). */
s_input *input_file_create(FILE *f, const char *filename);
/** @brief Delete a file input. */
void input_file_delete(s_input *input);
/** @brief Eat a char from a file. */
char input_file_getc(s_input *input);
/** @brief Get a char from a file. */
char input_file_topc(s_input *input);
/** @brief Not used by file input. */
int input_file_next(s_input *input, void *param);

#endif /* !INPUT_FILE_H */

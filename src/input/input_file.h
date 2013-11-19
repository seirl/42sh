#ifndef INPUT_FILE_H
# define INPUT_FILE_H

/** @brief eat a char from a file (input state = fd) */
char input_file_getc(void *input_state);
/** @brief get a char from a file (input state = fd) */
char input_file_topc(void *input_state);

#endif /* !INPUT_FILE_H */

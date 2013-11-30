#ifndef AUTOCOMPLETION_TOOLS_H
# define AUTOCOMPLETION_TOOLS_H

/**
** @brief Tell if a character is blank.
*/
int is_blank(char c);

/**
** @brief Return the position of the start of the word under the cursor.
*/
int start_word(s_term *term);

/**
** @brief Return the position of the end of the word under the cursor.
*/
size_t end_word(s_term *term);

/**
** @brief Tell if the word under the cursor is the first on the line.
*/
int is_first_word(s_term *term);

#endif /* !AUTOCOMPLETION_TOOLS_H */

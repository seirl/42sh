#ifndef SPECIAL_LINE_MANIPULATION_KEYS_H
# define SPECIAL_LINE_MANIPULATION_KEYS_H

# include "terminal.h"
# include "shell.h"
# include "key.h"

/**
** @brief Handle the Ctrl+a key
*/
e_next_action do_ctrl_a(s_shell *shell, s_term *term);

/**
** @brief Handle the Ctrl+e key
*/
e_next_action do_ctrl_e(s_shell *shell, s_term *term);

/**
** @brief Handle the Ctrl+k key
*/
e_next_action do_ctrl_k(s_term *term);

/**
** @brief Handle the Ctrl+l key
*/
e_next_action do_ctrl_l(s_term *term);

/**
** @brief Handle the Ctrl+c key
*/
e_next_action do_ctrl_c(s_term *term);

#endif /* !SPECIAL_LINE_MANIPULATION_KEYS_H */

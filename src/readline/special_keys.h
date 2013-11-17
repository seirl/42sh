#ifndef SPECIAL_KEYS_H
# define SPECIAL_KEYS_H

# include "terminal.h"
# include "key.h"

enum special_key
{
#define X(Name, Code) Name,
#include "special_keys.def"
#undef X
};
typedef enum special_key e_special_key;

e_next_action handle_special_char(s_term *term, char c);

#endif /* !SPECIAL_KEYS_H */

#ifndef KEY_H
# define KEY_H

# include "terminal.h"

enum key
{
#define X(Char, Name) Name,
#include "arrow.def"
#undef X
};
typedef enum key e_key;

char getkey(s_term *term);

#endif /* !KEY_H */

#ifndef KEY_H
# define KEY_H

enum key
{
#define X(Char, Name) Name,
#include "arrow.def"
#undef X
};
typedef enum key e_key;

char getkey();

#endif /* !KEY_H */

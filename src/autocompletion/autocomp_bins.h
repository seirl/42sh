#ifndef AUTOCOMP_BINS_H
# define AUTOCOMP_BINS_H

# include "terminal.h"

struct autocomp_binaries
{
    char ***bins;
    size_t size;
    size_t max_size;
};
typedef struct autocomp_binaries s_autocomp_binaries;

/**
** @brief Return the autocompleted word currently under the cursor.
*/
s_string *autocomp_bins(s_term *term);

#endif /* !AUTOCOMP_BINS_H */

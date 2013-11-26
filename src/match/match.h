#ifndef FNMATCH_H
# define FNMATCH_H

/**
** @brief The fnmatch() function checks whether the string argument matches the
** pattern argument, which is a shell wildcard pattern.
*/
int my_fnmatch(const char *pattern, const char *str);

typedef struct globr
{
    char **paths;
    unsigned count;
    size_t size_max;
} s_globr;


typedef enum glob_flags
{
    DOTGLOB,
    EXTGLOB,
    NOCASEGLOB,
    NULLGLOB,
} e_glob_flags;

/**
** The glob() function searches for all the pathnames matching pattern
** according to the rules used by the shell (see glob(7)).
*/
s_globr *my_glob(const char *pattern, e_glob_flags flags);

/**
** The globfree() function frees the dynamically allocated storage from an
** earlier call to glob().
*/
void my_globfree(s_globr *g);

#endif /* !FNMATCH_H */

#ifndef FNMATCH_H
# define FNMATCH_H

int my_fnmatch(const char *pattern, const char *str);
char **my_glob(const char *pattern, int *count);

#endif /* !FNMATCH_H */

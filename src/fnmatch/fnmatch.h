#ifndef MY_FNMATCH_H
# define MY_FNMATCH_H

int my_fnmatch(const char *pattern, const char *str);
char **my_glob(const char *pattern, int *count);

#endif /* !MY_FNMATCH_H */

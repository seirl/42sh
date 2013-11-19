#ifndef SHOPT_H
# define SHOPT_H

# include "getopt.h"

struct shopt
{
    char *name;
    int value;
};
typedef struct shopt s_shopt;

int shopt_set(const char *shopt, int value);
int shopt_get(const char *shopt);
void shopt_print(void);
int shopt_from_opt(s_opt *opt);

#endif /* !SHOPT_H */

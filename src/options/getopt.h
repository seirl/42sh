#ifndef GETOPT_H
# define GETOPT_H

# include "macros.h"

enum type
{
    e_int,
    e_str,
    e_void
};
typedef enum type e_type;

struct arg
{
    union
    {
        char *str;
        int n;
        double f;
    } value;
    int set;
    struct arg *next;
};
typedef struct arg s_arg;

struct param
{
    char short_name;
    char *long_name;
    int multi;
    e_type type;
    s_arg arg;
};

typedef struct param s_param;

struct opt
{
    unsigned int count;
# define X(S, L, T, M) + 1
    s_param param[1 +
# include "arg.def"
    ];
# undef X
    char **trailing;
    unsigned int trailing_count;
};

typedef struct opt s_opt;

int opt_parse(int argc, char *argv[], s_opt *opt);
int opt_get(s_opt *opt, const char *s, void *res);
int opt_is_set(s_opt *opt, const char *arg, const char *name);
char *opt_trailing_arg(s_opt *opt, unsigned int i);
void opt_free(s_opt *opt);

#endif /* !GETOPT_H */

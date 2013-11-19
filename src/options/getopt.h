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

/** @brief an argument entry */
struct arg
{
    //! the value
    union
    {
        char *str;
        int n;
        double f;
    } value;
    //! is set ?
    int set;
    //! linked lists
    struct arg *next;
};
typedef struct arg s_arg;

/** @brief a valid param according to the program definition */
struct param
{
    //! short name (-x)
    char short_name;
    //! long name (--help)
    char *long_name;
    //! appear multiple time (+O -O)
    int multi;
    //! type of the expected value
    e_type type;
    //! the argument
    s_arg arg;
};

typedef struct param s_param;

/** @breif all the parsed options */
struct opt
{
    //! number of arguments
    unsigned int count;
    //! array containing the arguments
# define X(S, L, T, M) + 1
    s_param param[1 +
# include "arg.def"
    ];
# undef X
    //! trailing argument
    char **trailing;
    //! number of trailing argument
    unsigned int trailing_count;
};

typedef struct opt s_opt;

/** @brief parse the command line and properly fill an opt struct */
int opt_parse(int argc, char *argv[], s_opt *opt);
/** @brief get the value of a short or long option */
int opt_get(s_opt *opt, const char *s, void *res);
/** @brief return the value of a +/- option */
int opt_is_set(s_opt *opt, const char *arg, const char *name);
/** @brief get the ith trailling argument (a file, ...) */
char *opt_trailing_arg(s_opt *opt, unsigned int i);
/** @brief free an opt structure */
void opt_free(s_opt *opt);

#endif /* !GETOPT_H */

#include "getopt.h"
#include "shopt.h"
#include "macros.h"

static void version()
{
    fprintf(stdout, "Version "VERSION"\n");
}

static void usage()
{
    fprintf(stdout,
        "Usage: "PROGNAME" [options] ...\n"
        "       "PROGNAME" [options] script-file ...\n"
        "Shell options:\n"
        "        -c command     Execute the given command\n"
        "        [+-]O shopt    Set/Unset a shopt variable\n"
        "        --norc         Deactivate the resource reader\n"
        "        --ast-print    Display the Abstract Sytax Tree (AST)\n"
        "        --version      Diplay the current version number\n"
        "        --help         Display this help\n");

}

int parse_options(int argc, char *argv[])
{
    s_opt opt;
    char *arg;
    if (opt_parse(argc, argv, &opt) == 1)
    {
        usage();
        opt_free(&opt);
        return 1;
    }
    if (opt_get(&opt, "help", NULL))
        usage();
    if (opt_get(&opt, "version", NULL))
        version();
    if (opt_get(&opt, "norc", NULL))
    {
        ;
    }
    if (opt_get(&opt, "ast-print", NULL))
    {
        ;
    }
    if (opt_get(&opt, "c", &arg))
    {
        printf("Exec %s\n", arg);
    }
    shopt_from_opt(&opt);
    opt_free(&opt);
    return 0;
}
#include "getopt.h"
#include "shopt.h"
#include "macros.h"

static void version(void)
{
    fprintf(stdout, "Version "VERSION"\n");
}

static void usage(FILE *output)
{
    fprintf(output,
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

static int check_args(s_shell *shell, s_opt opt)
{
    if (opt_get(&opt, "help", NULL))
    {
        usage(stdout);
        return 3;
    }
    if (opt_get(&opt, "version", NULL))
    {
        version();
        return 3;
    }
    if (opt_get(&opt, "ast-print", NULL))
        shopt_set(shell, "ast_print", 1);
    if (opt_get(&opt, "token-print", NULL))
        shopt_set(shell, "token_print", 1);
    return opt_get(&opt, "norc", NULL) == 0;
}

// TODO: use an enum as the return value
int parse_options(s_shell *shell, int argc, char *argv[], char **cmd, char **file)
{
    s_opt opt;
    char *arg;
    int ret = opt_parse(argc, argv, &opt);
    if (ret)
    {
        if (ret == 1)
            usage(stderr);
        opt_free(&opt);
        return 2;
    }
    if (shopt_from_opt(shell, &opt) == 1)
    {
        opt_free(&opt);
        return 2;
    }
    ret = check_args(shell, opt);
    if (opt_get(&opt, "c", &arg))
        *cmd = arg;
    if (opt_trailing_arg(&opt, 0))
        *file = opt_trailing_arg(&opt, 0);
    opt_free(&opt);
    return ret;
}

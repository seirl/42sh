#include "options.h"
#include "getopt.h"
#include "shopt.h"
#include "macros.h"

static const s_param main_param[] =
{
    { 'c', NULL, 0, 1, NEW_ARG },
    { 'O', NULL, 1, 1, NEW_ARG },
    { '/', "norc", 0, 0, NEW_ARG },
    { '/', "ast-print", 0, 0, NEW_ARG },
    { '/', "token-print", 0, 0, NEW_ARG },
    { '/', "version", 0, 0, NEW_ARG },
    { '/', "help", 0, 0, NEW_ARG }
};

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
        return E_RET;
    }
    if (opt_get(&opt, "version", NULL))
    {
        version();
        return E_RET;
    }
    if (opt_get(&opt, "ast-print", NULL))
        shopt_set(shell, "ast_print", 1);
    if (opt_get(&opt, "token-print", NULL))
        shopt_set(shell, "token_print", 1);
    return opt_get(&opt, "norc", NULL) ? E_RC : 0;
}

e_option_return parse_options(s_shell *shell, int argc, char *argv[],
                             char **source)
{
    s_opt *opt = opt_init(main_param, 7);
    char *arg;
    int ret = opt_parse(argc, argv, opt);
    if (ret)
    {
        if (ret == 1)
            usage(stderr);
        opt_free(opt);
        return E_ERROR;
    }
    if (shopt_from_opt(shell, opt) == 1)
    {
        opt_free(opt);
        return E_ERROR;
    }
    ret = check_args(shell, *opt);
    if (opt_get(opt, "c", &arg))
    {
        *source = arg;
        ret += E_STR;
    }
    else if (opt_trailing_arg(opt, 0))
    {
        *source = opt_trailing_arg(opt, 0);
        ret += E_FILE;
    }
    opt_free(opt);
    return ret;
}

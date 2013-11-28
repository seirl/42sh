#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctype.h>
#include <unistd.h>
#include "env.h"
#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "shell_private.h"
#include "expand.h"
#include "match.h"

static char *get_home(s_shell *shell, char *user)
{
    char *home = NULL;
    struct passwd *pw;

    if (user == NULL)
    {
        home = env_get(shell, "HOME");
        if (home)
            return home;
    }

    pw = user == NULL ? getpwuid(getuid()) : getpwnam(user);
    if (pw == NULL)
        return NULL;
    return pw->pw_dir;
}

static int extract_username_path(char *str, s_string **user, s_string **path)
{
    *user = string_create(0);
    *path = string_create(0);
    int base = 0;
    for (int i = 0; str[i]; ++i)
    {
        if (base == 0)
        {
            if (islower(str[i]) || (i && isdigit(str[i])) || str[i] == '_')
                string_putc(*user, str[i]);
            else if (str[i] == '/')
            {
                base = 1;
                string_putc(*path, str[i]);
            }
            else
            {
                string_free(*user);
                string_free(*path);
                return 0;
            }
        }
        else
            string_putc(*path, str[i]);
    }
    return 1;
}

static int user_tilde(s_shell *shell, s_string *word, s_string *ret)
{
    s_string *user = NULL;
    s_string *path = NULL;
    if (extract_username_path(word->buf + 1, &user, &path) == 0)
        return 0;
    char *home = get_home(shell, user->buf);
    if (home == NULL)
    {
        string_free(user);
        string_free(path);
        return 0;
    }
    string_puts(ret, home);
    string_puts(ret, path->buf);
    string_free(user);
    string_free(path);
    return 1;
}

static int simple_tilde(s_shell *shell, s_string *word, s_string *ret)
{
    char c = string_topc(word);
    if (c == 0 || c == '/')
    {
        string_puts(ret, get_home(shell, NULL));
        return 1;
    }
    return 0;
}

static int operator_tilde(s_shell *shell, s_string *word, s_string *ret)
{
    char c = string_topc(word);
    char c2 = string_topc_n(word, 1);
    if (c2 != 0 && c2 != '/')
        return 0;
    if (c == '+')
        string_puts(ret, env_get(shell, "PWD"));
    else if (c == '-')
        string_puts(ret, env_get(shell, "OLDPWD"));
    else
        return 0;
    string_getc(word);
    if (c2 == '/')
        string_getc(word);
    return 1;
}

static s_string *tilde_expansion(s_shell *shell, s_string *word)
{
    char c;
    char prev = 0;
    s_string *ret = string_create(0);
    s_string *word_cpy = string_duplicate(word);
    for (c = string_getc(word_cpy); c; c = string_getc(word_cpy))
    {
        if (c == '~' && prev != '~' && prev != '\\')
        {
            if (simple_tilde(shell, word_cpy, ret))
                continue;
            if (operator_tilde(shell, word_cpy, ret))
                continue;
            if (user_tilde(shell, word_cpy, ret))
                return ret;
        }
        string_putc(ret, c);
        prev = c;
    }
    string_free(word_cpy);
    return ret;
}

s_string *expand_simple_word(s_shell *shell, s_string *word)
{
    return tilde_expansion(shell, word);
}

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
        return "";
    return pw->pw_dir;
}

static int check_username_charset(char *str)
{
    for (int i = 0; str[i]; ++i)
    {
        char c = str[i];
        if (islower(c) || (i && isdigit(c)) || c == '_')
            continue;
        return 0;
    }
    return 1;
}

static int user_tilde(s_shell *shell, s_string *word, s_string *ret)
{
    if (check_username_charset(word->buf + 1) == 0)
        return 0;
    s_string *user = string_create(16);
    int offset = 0;
    for (char *str = word->buf + word->read_pos; *str; ++str)
    {
        if (*str == '/')
            break;
        string_putc(user, *str);
        ++offset;
    }
    char *home = get_home(shell, user->buf);
    if (home == NULL)
    {
        string_free(user);
        return 0;
    }
    string_puts(ret, home);
    string_free(user);
    word->read_pos += offset;
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
        if (c == '~' && prev != '~')
        {
            if (simple_tilde(shell, word_cpy, ret))
                continue;
            if (operator_tilde(shell, word_cpy, ret))
                continue;
            if (user_tilde(shell, word_cpy, ret))
                continue;
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

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include "env.h"
#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"

static char *get_home(const s_shell *shell, char *user)
{
    (void)shell;
    char *home = NULL;
    struct passwd *pw;

    if (user == NULL)
    {
        home = env_get("HOME");
        if (home)
            return home;
    }

    pw = user == NULL ? getpwuid(getuid()) : getpwnam(user);
    if (pw == NULL)
        return "";
    return pw->pw_dir;
}

static int user_tilde(const s_shell *shell, s_string *word, s_string *ret)
{
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

static int simple_tilde(const s_shell *shell, s_string *word, s_string *ret)
{
    char c = string_topc(word);
    if (c == 0 || c == '/')
    {
        string_puts(ret, get_home(shell, NULL));
        return 1;
    }
    return 0;
}

static int operator_tilde(const s_shell *shell, s_string *word, s_string *ret)
{
    (void)shell;
    char c = string_topc(word);
    char c2 = string_topc_n(word, 1);
    if (c2 != 0 && c2 != '/')
        return 0;
    if (c == '+')
        string_puts(ret, env_get("PWD"));
    else if (c == '-')
        string_puts(ret, env_get("OLDPWD"));
    else
        return 0;
    string_getc(word);
    if (c2 == '/')
        string_getc(word);
    return 1;
}

static s_string *tilde_expansion(const s_shell *shell, const s_string *word)
{
    char c;
    s_string *ret = string_create(0);
    s_string *word_cpy = string_duplicate(word);
    for (c = string_getc(word_cpy); c; c = string_getc(word_cpy))
    {
        if (c == '~')
        {
            if (simple_tilde(shell, word_cpy, ret))
                continue;
            if (operator_tilde(shell, word_cpy, ret))
                continue;
            if (user_tilde(shell, word_cpy, ret))
                continue;
        }
        string_putc(ret, c);
    }
    string_free(word_cpy);
    return ret;
}

s_string *expand_simple_word(const s_shell *shell, const s_string *word)
{
    return tilde_expansion(shell, word);
}

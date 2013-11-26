#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include "string_utils.h"
#include "env.h"

static size_t len_until(char *s, char c)
{
    size_t i;
    for (i = 0; s[i] && s[i] != c; i++)
        continue;
    return i;
}

size_t prompt_replace(s_string *prompt, size_t pos, size_t len, s_string *rep)
{
    char *tmp = strdup(prompt->buf + pos + 1 + len);
    size_t replaced_len = rep->len;
    string_del_from_end(prompt, prompt->len - pos);
    string_puts(prompt, rep->buf);
    string_puts(prompt, tmp);
    free(tmp);
    string_free(rep);
    // TODO: something is wrong with that.
    return replaced_len;
}

static s_string *get_time(s_string *prompt, size_t pos)
{
    char *format = "%a %b %d";
    if (prompt)
    {
        format = malloc(sizeof (char) * (prompt->len + 2));
        size_t end_pos = pos;
        while (prompt->buf[end_pos] && prompt->buf[end_pos] != '}')
            end_pos++;
        strncpy(format, prompt->buf + pos, end_pos - pos);
        format[end_pos - pos] = 0;
    }

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (!t)
        return string_create(1);

    s_string *date = string_create(100);
    strftime(date->buf, 100, format, tm);

    if (prompt)
        free(format);

    return date;
}

static s_string *get_hostname_until(s_shell *shell, char c)
{
    char *hostname = env_get(shell, "HOSTNAME");
    if (!hostname)
        return string_create(1);

    s_string *res = string_create(strlen(hostname));
    strncpy(res->buf, hostname, len_until(hostname, c));
    return res;
}

static s_string *get_w(s_shell *shell, int use_basename)
{
    s_string *w;
    if (use_basename)
        w = string_create_from(basename(env_get(shell, "PWD")));
    else
        w = string_create_from(env_get(shell, "PWD"));
    if (!w->buf)
        return w;

    if (env_get(shell, "HOME"))
        string_replace(w, env_get(shell, "HOME"), "~");
    return w;
}

void prompt_expand(s_shell *shell, s_string *prompt)
{
    (void)shell;
    for (size_t i = 0; prompt->buf[i + 1]; i++)
    {
        if (prompt->buf[i] == '\\')
        {
#define X(Name, Len, Pattern_len, Rep)                              \
            if  (!strncmp(prompt->buf + i + 1, Name, Len))          \
            {                                                       \
                i += prompt_replace(prompt, i, Pattern_len, Rep);   \
                continue;                                           \
            }
#include "prompt.def"
#undef X
        }
    }
}

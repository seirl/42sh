#include <ctype.h>

static const struct sclasses
{
    char *name;
    int (*checker)(int c);
} classes[] =
{
    { "alnum", isalnum },
    { "alpha", isalpha },
    { "blank", isblank },
    { "cntrl", iscntrl },
    { "digit", isdigit },
    { "graph", isgraph },
    { "lower", islower },
    { "print", isprint },
    { "punct", ispunct },
    { "space", isspace },
    { "upper", isupper },
    { "xdigit", isxdigit },
};

static int classeq(const char *start, int class_index)
{
    for (int i = 0; start[i] && start[i] != ':' && start[i + 1] != ']'; i++)
    {
        if (start[i] != classes[class_index].name[i])
            return 0;
    }
    return 1;
}

static int match_class(const char *current, int *i, const char c)
{
    *i += 2;
    int r = 0;
    for (unsigned int j = 0; j < sizeof (classes) / sizeof (classes[0]); j++)
        if (classeq(current + *i, j))
        {
            r = !!classes[j].checker(c);
            break;
        }
    while (current[*i] && (current[*i] != ']' || current[*i - 1] != ':'))
        (*i)++;
    return r;
}

static int match_range(const char **range, const char c)
{
    const char *cur = *range + 1;
    int negation = (*cur == '!' && ++cur) ? 1 : 0;
    int r = 0;
    int i = 0;
    for (i = 0; cur[i] && !(cur[i] == ']' && i); ++i)
    {
        if (cur[i] == '-' && i && cur[i + 1] && cur[i + 1] != ']')
        {
            if (!r && c >= cur[i - 1] && c <= cur[i + 1])
                r = 1;
            i++;
        }
        else if (!r && cur[i] == '[' && cur[i + 1] == ':')
            r = match_class(cur, &i, c);
        else if (!r && cur[i] == c)
            r = 1;
    }
    *range = cur + i;
    return (negation) ? !r : r;
}

int my_fnmatch(const char *p, const char *str)
{
    for (; *str; str++, p++)
    {
        if (*p == '*')
            return !!p[1] && my_fnmatch(p + 1, str) && my_fnmatch(p, str + 1);
        else if (*p == '[')
        {
            if (!match_range(&p, *str))
                return 1;
        }
        else if (*p == '\\')
        {
            if (*++p != *str)
                return 1;
        }
        else if (*p != '?' && *p != *str)
            return 1;
    }
    while (*p == '*')
        p++;
    return !!p[0];
}

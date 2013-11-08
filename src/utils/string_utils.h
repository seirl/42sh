#ifndef STRING_BUF_H
# define STRING_BUF_H

# include <stddef.h>

struct string
{
    char *buf;
    size_t len;
    size_t max_len;
};

typedef struct string s_string;

s_string *string_create(size_t size);
s_string *string_create_from(char *str);
void string_resize(s_string *s);
char *string_release(s_string *s);
void string_free(s_string *s);

void string_putc(s_string *s, char c);
void string_puts(s_string *s, char *str);
void string_cat(s_string *s1, s_string *s2);
void string_ungetc(s_string *s);

void string_replace(s_string *s, char *pattern, char *rep);

#endif /* !STRING_BUF_H */

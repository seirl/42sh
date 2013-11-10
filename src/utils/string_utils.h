#ifndef STRING_BUF_H
# define STRING_BUF_H

# include <stddef.h>

struct string
{
    char *buf;
    size_t read_pos;
    size_t len;
    size_t max_len;
};

typedef struct string s_string;

s_string *string_create(size_t size);
s_string *string_create_from(const char *str);
void string_resize(s_string *s);
//! @brief Set string size to 0.
void string_reset(s_string *s);
char *string_release(s_string *s);
void string_free(s_string *s);

void string_putc(s_string *s, char c);
void string_puts(s_string *s, const char *str);
void string_cat(s_string *s1, s_string *s2);
s_string *string_vcat(const char *s1, ...);

char string_getc(s_string *s);
void string_ungetc(s_string *s);
void string_rewind(s_string *s);
void string_eat_spaces(s_string *s);
int string_eat_pattern(s_string *s, char *pattern);

void string_replace(s_string *s, char *pattern, char *rep);

int string_equal(const s_string *s, const char *pattern);

char *string_nullterminated(s_string *s);

#endif /* !STRING_BUF_H */

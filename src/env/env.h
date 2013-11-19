#ifndef ENV_H
# define ENV_H

void env_set(char *name, char *value);
char *env_get(char *name);
void env_unset(char *name);
void env_free(void);

#endif /* !ENV_H */

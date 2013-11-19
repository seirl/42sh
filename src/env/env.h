#ifndef ENV_H
# define ENV_H

/** @brief add a environnment variable, or change its value */
void env_set(char *value, char *name);
/** @brief get the value (or NULL) of an environnment variable */
char *env_get(char *name);
/** @brief unset an environnment variable */
void env_unset(char *name);
/** @brief free the environnment (unset all the variables) */
void env_free(void);

#endif /* !ENV_H */

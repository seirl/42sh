#ifndef SMALLOC_H
# define SMALLOC_H

# include <stdlib.h>
# define N_BUCKETS 100

union smalloc_bucket
{
    union smalloc_bucket *next;
    size_t padding[1];
};
typedef union smalloc_bucket u_smalloc_bucket;

void *smalloc(size_t size);
void *scalloc(size_t nmemb, size_t size);
void *srealloc(void *ptr, size_t size);
void sfree(void *ptr);
void smalloc_clean(void);

#endif /* !SMALLOC_H */

#include <stdlib.h>

#include "log.h"
#include "stack.h"

s_numstack *numstack_init(void)
{
    s_numstack *o = malloc(sizeof (s_numstack));
    o->size_max = 8;
    o->size = 0;
    o->nums = malloc(sizeof (int) * o->size_max);
    return o;
}

void numstack_push(s_numstack *s, int num)
{
    if (s->size >= s->size_max)
    {
        s->size_max *= 2;
        s->nums = realloc(s->nums, sizeof (int) * s->size_max);
    }
    s->nums[s->size++] = num;
}

int numstack_pop(s_numstack *s)
{
    if (!s->size)
    {
        LOG(ERROR, "%s", "Number stack empty");
        return 0;
    }
    return s->nums[--s->size];
}

void numstack_free(s_numstack *s)
{
    free(s->nums);
    free(s);
}

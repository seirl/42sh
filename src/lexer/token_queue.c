#include "token_queue.h"
#include "smalloc.h"

s_token_queue *token_queue_create()
{
    s_token_queue *q;
    q = smalloc(sizeof (s_token_queue));
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void token_enqueue(s_token_queue *q, s_token *tok)
{
    tok->next = NULL;
    if (q->size == 0)
    {
        q->head = tok;
        q->tail = tok;
    }
    else
    {
        q->tail->next = tok;
        q->tail = tok;
    }
    q->size += 1;
}

s_token *token_front(s_token_queue *q)
{
    return q->head;
}

s_token *token_dequeue(s_token_queue *q)
{
    s_token *ret;
    if (q->size == 0)
    {
        ret = smalloc(sizeof (s_token));
        ret->type = T_EOF;
        ret->value = NULL;
        return ret;
    }
    ret = token_front(q);
    q->head = q->head->next;
    q->size -= 1;
    return ret;
}

void token_queue_free(s_token_queue *q)
{
    sfree(q);
}

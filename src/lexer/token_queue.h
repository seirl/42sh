#ifndef TOKEN_QUEUE_H
# define TOKEN_QUEUE_H

# include "token.h"

struct token_queue
{
    s_token *head;
    s_token *tail;
    size_t size;
};
typedef struct token_queue s_token_queue;

s_token_queue *token_queue_create();
void token_enqueue(s_token_queue *q, s_token *tok);
s_token *token_front(s_token_queue *q);
s_token *token_dequeue(s_token_queue *q);
void token_queue_free(s_token_queue *q);

#endif /* !TOKEN_QUEUE_H */

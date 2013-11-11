#ifndef LIST_H
# define LIST_H

# define LIST(Type, Name)                                              \
    typedef struct Node_##Name s_Node_##Name;                          \
    struct Node_##Name                                                 \
    {                                                                  \
        Type data;                                                     \
        s_Node_##Name *next;                                           \
        s_Node_##Name *prev;                                           \
    };                                                                 \
    struct Name                                                        \
    {                                                                  \
        unsigned size;                                                 \
        s_Node_##Name *head;                                           \
        s_Node_##Name *tail;                                           \
        s_Node_##Name *it;                                             \
        void(*free)(Type);                                             \
    };                                                                 \
    typedef struct Name s_##Name

# define LIST_INIT(List, F)                                            \
    do {                                                               \
        List = calloc(1, sizeof (*List));                              \
        List->free = F;                                                \
    } while(0)                                                         \

# define LIST_FREE(List)                                               \
    do {                                                               \
        while (List->head)                                             \
        {                                                              \
            List->tail = List->head;                                   \
            List->head = List->head->next;                             \
            if (List->free)                                            \
                List->free(List->tail->data);                          \
            free(List->tail);                                          \
        }                                                              \
        free(List);                                                    \
    } while(0)

# define LIST_INSERT_AFTER(List, Val, Node)                            \
    do {                                                               \
        if (Node)                                                      \
        {                                                              \
            void *nextNode = Node->next;                               \
            Node->next = malloc(sizeof (*Node));                       \
            Node->next->data = Val;                                    \
            Node->next->next = nextNode;                               \
            Node->next->prev = Node;                                   \
            if (Node->next->next)                                      \
                Node->next->next->prev = Node->next;                   \
        }                                                              \
        else                                                           \
        {                                                              \
            List->head = calloc(1, sizeof (*Node));                    \
            List->head->data = Val;                                    \
        }                                                              \
        if (Node == List->tail)                                        \
            List->tail = Node ? Node->next : List->head;               \
        List->size++;                                                  \
    } while(0)

# define LIST_INSERT_BEFORE(List, Val, Node)                           \
    do {                                                               \
        if (Node)                                                      \
        {                                                              \
            void *prevNode = Node->prev;                               \
            Node->prev = malloc(sizeof (*Node));                       \
            Node->prev->data = Val;                                    \
            Node->prev->prev = prevNode;                               \
            Node->prev->next = Node;                                   \
            if (Node->prev->prev)                                      \
                Node->prev->prev->next = Node->prev;                   \
        }                                                              \
        else                                                           \
        {                                                              \
            List->tail = calloc(1, sizeof (*Node));                    \
            List->tail->data = Val;                                    \
        }                                                              \
        if (Node == List->head)                                        \
            List->head = Node ? Node->prev : List->tail;               \
        List->size++;                                                  \
    } while(0)

# define LIST_INSERT_TAIL(List, Val)                                   \
    do {                                                               \
        LIST_INSERT_AFTER(List, Val, List->tail);                      \
        if (!List->head)                                               \
            List->head = List->tail;                                   \
    } while(0)

# define LIST_INSERT_HEAD(List, Val)                                   \
    do {                                                               \
        LIST_INSERT_BEFORE(List, Val, List->head);                     \
        if (!List->tail)                                               \
            List->tail = List->head;                                   \
    } while(0)

# define LIST_AT(List, N, Elt)                                         \
    do {                                                               \
        int i;                                                         \
        Elt = List->head;                                              \
        for (i = 0; i < N && Elt; ++i)                                 \
            Elt = Elt->next;                                           \
    } while(0)

# define LIST_FOREACH(List)                                            \
    for (List->it = List->head; List->it; List->it = List->it->next)

# define LIST_INSERT_AT(List, Val, n)                                  \
    do {                                                               \
        if (n < List->size)                                            \
        {                                                              \
            List_AT(List, n, List->it);                                \
            List_INSERT_BEFORE(List, Val, List->it);                   \
        }                                                              \
        else                                                           \
            List_INSERT_TAIL(List, Val);                               \
    } while(0)

# define LIST_REMOVE(List, Node)                                       \
    do {                                                               \
        if (Node->next)                                                \
            Node->next->prev = Node->prev;                             \
        if (Node->prev)                                                \
            Node->prev->next = Node->next;                             \
        if (Node == List->head)                                        \
            List->head = Node->next;                                   \
        if (Node == List->tail)                                        \
            List->tail = Node->prev;                                   \
        if (List->free)                                                \
            List->free(Node->data);                                    \
        free(Node);                                                    \
        List->size--;                                                  \
    } while(0)

# define LIST_REMOVE_AT(List, N)                                       \
    do {                                                               \
        if (N < List->size)                                            \
        {                                                              \
            LIST_AT(List, N, List->it);                                \
            LIST_REMOVE(List, List->it);                               \
        }                                                              \
    } while(0)

# define LIST_REMOVE_HEAD(List)                                        \
    do {                                                               \
        List->it = List->head;                                         \
        LIST_REMOVE(List, List->it);                                   \
    } while(0)

#endif /* !LIST_H */

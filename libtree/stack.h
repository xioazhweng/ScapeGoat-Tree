#ifndef STACK_H
#define STACK_H
#include "bsttree.h"

typedef struct StackNode {
    Node * leaf;
    unsigned level;
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* head;
} Stack;
typedef enum ErrStack {
    ERR_OK_STACK = 0,
    ERR_EOF_STACK = 1,
    ERR_MEMORY_STACK = 2,
    ERR_EXIST_STACK = 3,
    ERR_UNDERFLOW_STACK = 4,
    ERR_OVERFLOW_STACK = 5,
} ErrStack;

Stack*   create_stack(void);
ErrStack push_stack(Stack* const q, Node * pleaf, unsigned level);
ErrStack pop_stack(Stack* const q, StackNode* const ppleaf);
void     print_stack(const Stack* const q);
void     delete_stack(Stack * q);
ErrStack is_empty_stack(const Stack* const q);
void     check_status_stack(const Stack *q);

#endif //STACK_H

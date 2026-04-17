#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "bsttree.h"
#include "internal.h"

Stack* create_stack(void) {
    return (Stack*) calloc(1, sizeof(Stack));
}

ErrStack push_stack(Stack* const q, Node * pleaf, unsigned level) {
    StackNode* element = (StackNode*) calloc(1, sizeof(StackNode));
    if (element == NULL)
    {
        return ERR_MEMORY_STACK;
    }
    element->leaf = pleaf;
    element->next = q->head;
    element->level = level;
    q->head = element;
    return ERR_OK_STACK;
}

ErrStack pop_stack(Stack* const q, StackNode* const p) {
    if (q->head == NULL) {
        return ERR_UNDERFLOW_STACK;
    }
    StackNode* top = q->head;
    p->leaf = top->leaf;
    p->level = top->level;
    q->head = top->next;
    free(top);
    return ERR_OK_STACK;
}

void print_stack(const Stack* const q) {
    StackNode* pstack = q->head;
    while (pstack != NULL) {
        print_leaf(pstack->leaf);
        pstack = pstack->next;
    }
    printf("\n");
}

void delete_stack(Stack * q) {
    StackNode* pstack = q->head;
    StackNode* elem;
    while (pstack != NULL) {
        elem = pstack;
        pstack = pstack->next;
        free(elem);
    }
    free(q);
}
ErrStack is_empty_stack(const Stack* const q) {
    if (q->head == NULL){
        return ERR_UNDERFLOW_STACK;
    }
    return ERR_OK_STACK;
}

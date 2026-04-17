#include <stdio.h>
#include "vect.h"
#include "bsttree.h"
#include "internal.h"
#include <stdlib.h>
#define DEBUG 0

Vect* create_vect(size_t elem_size) {
    Vect * qvect = (Vect *) calloc(1, sizeof(Vect));
    if (qvect == NULL) {
        return NULL;
    }
    qvect->data = (void *) calloc(INIT_VECT_SIZE, elem_size);
    if (qvect->data == NULL) {
        free(qvect);
        return NULL;
    }
    qvect->elem_size = elem_size;
    qvect->size = INIT_VECT_SIZE;
    qvect->cap = 0;
    return qvect;
}

ErrVect append_vect(Vect * const pvect, const void * const elem) {
    if (elem == NULL) {
        return ERR_VECT_MEM;
    }
    ErrVect mes = ERR_VECT_OK;
    if (pvect->size == pvect->cap) {
        mes = expansion_vect(pvect);
    }
    if (mes == ERR_VECT_MEM) {
        return mes;
    }
    Data* p = (Data *) elem;
    Data* data = pvect->data;
    Node* prev, * next;
    set_leaf(data + pvect->cap, p->key, *p->info);
    prev = (data + pvect->cap);
    for (Node * q = p->next; q != NULL; q = q->next) {
        Node * pnode = create_leaf();
        if (pnode == NULL) {
           Node * q = (data + pvect->cap)->next;
           while (q != NULL) {
                next = q->next;
                free(q->info);
                free(q);
                q = next;
            }
            memset_leaf(data + pvect->cap);
            return ERR_VECT_MEM;
        }
        set_leaf(pnode, q->key, *q->info);
        prev->next = pnode;
        prev = pnode;
    }
    pvect->cap++;
    return mes;
}

void delete_vect(Vect* pvect) {
    if (pvect == NULL || pvect->data == NULL) {
        return;
    }
    Node * next;
    Data* data = pvect->data;
    for (unsigned i = 0; i < pvect->cap; i++) {
        Node * q = (data + i)->next;
        while (q != NULL) {
            next = q->next;
            free(q->info);
            free(q);
            q = next;
        }
        free(data[i].info);
    }
    free(pvect->data);
    free(pvect);
}

ErrVect expansion_vect(Vect * const pvect) {
    void * p  = realloc(pvect->data, pvect->elem_size * pvect->size * VECT_COEF);
    if (p == NULL) {
        return ERR_VECT_MEM;
    }
    pvect->data = p;
    pvect->size *= VECT_COEF;
    return ERR_VECT_OK;
}

void print_vect(const Vect* const vec) {
    if (vec == NULL) {
        printf("Error: Vector pointer is NULL\n");
        return;
    }
    if (vec->data == NULL || vec->cap == 0) {
        printf("Vector is empty\n");
        return;
    }

    printf("\nsize: %u, capasity: %u\n", vec->size, vec->cap);
    print_table_leaf_header();
    Data* data = vec->data;
    for (unsigned i = 0; i < vec->cap; i++) {
        print_leaf(data + i);
    }
    print_table_leaf_footer();
}

#ifndef VECT_H
#define VECT_H
#include <stddef.h>
#define INIT_VECT_SIZE 2u
#define VECT_COEF 2u

typedef struct Node Data;
typedef struct Vect {
    void * data;
    unsigned cap;
    size_t elem_size;
    unsigned size;
} Vect;

typedef enum ErrVect {
    ERR_VECT_OK = 0,
    ERR_VECT_MEM,
} ErrVect;

Vect*   create_vect(size_t elem_size);
ErrVect append_vect(Vect * const pvect, const void * const elem);
ErrVect expansion_vect(Vect * const pvect);
void    delete_vect(Vect* pvect);
void    print_vect(const Vect* const pvect);

#endif

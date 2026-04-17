#include <stdlib.h>
#include <stdio.h>
#include "bsttree.h"
#include "internal.h"
#include "vect.h"
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define N 2
#define KEYS_AMOUNT 1000u
#define TEST_AMOUNT 20u
#define NODES_AMOUNT 800000u

ErrTree timing_set(double * const time);
ErrTree timing_search(double * const time);
ErrTree timing_in_order(double * const time);
ErrTree timing_delete(double * const time);
typedef ErrTree (*TimingFunc)(double * const);
TimingFunc f_timing_ptr[4] = {
    timing_set,
    timing_search,
    timing_delete,
    timing_in_order,

};

ErrTree timing_tree(const char * filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return ERR_TREE_FILE_FAIL;
    }

    double time[4][TEST_AMOUNT] = {0};
    ErrTree msg = ERR_TREE_OK;
    for (int i = 0; i < 4; i++) {

        msg = f_timing_ptr[i](time[i]);
        if (msg != ERR_TREE_OK) {
            goto end;
        }
    }
    for (int test_index = TEST_AMOUNT - 1; test_index >= 0; test_index--)  {
         unsigned nodes = (TEST_AMOUNT - test_index) * NODES_AMOUNT;
         fprintf(file, "%u ", nodes);
         for (int i = 0; i < 4; i++) {
             fprintf(file, "%lf", time[i][test_index]);
             if (i == 3) {
                 fprintf(file, "\n");
            } else {
                fprintf(file, " ");
            }

        }
    }
    end:
    fclose(file);
    return msg;
}
ErrTree timing_set(double * const time) {
    srand(rand());
    BinaryTree * tree = create_tree();
    if (tree == NULL) {
        return ERR_TREE_MEM;
    }
    printf("\nSET TEST\n");
    struct timeval first, last;
    ErrTree msg = ERR_TREE_OK;
    for (int jndex = 0; jndex < N; jndex++)  {
        memset_tree(tree);
        for (int test_index = TEST_AMOUNT - 1; test_index >= 0; test_index--) {
            for (int i = 0; i < NODES_AMOUNT; i++) {
                set_tree(tree, rand () * rand(), 0);
            }
            gettimeofday(&first, NULL);
            for (unsigned i = 0; i < KEYS_AMOUNT; ) {
                if (set_tree(tree, rand() * rand(), 0) == ERR_TREE_OK) {
                    i++;
                }
            }
            gettimeofday(&last, NULL);
            time[test_index]  += (double) (last.tv_usec - first.tv_usec) / 1000000 +  (double)   (last.tv_sec - first.tv_sec);
        }
    }
     for (int test_index = TEST_AMOUNT - 1; test_index >= 0; test_index--) {
         time[test_index] /= N;
         printf("test #%d, number of nodes = %u, time = %lf\n", TEST_AMOUNT - test_index,
                (TEST_AMOUNT - test_index) * NODES_AMOUNT, time[test_index]);
    }
    delete_tree(tree);
   return msg;
}
ErrTree timing_search(double * const time) {
    srand(rand());
    BinaryTree * tree = create_tree();
    if (tree == NULL) {
        return ERR_TREE_MEM;
    }
    struct timeval first, last;
    ErrTree msg = ERR_TREE_OK;
    unsigned keys[KEYS_AMOUNT], found;
    printf("\nSEARCH TEST\n");
    for (int test_index = TEST_AMOUNT - 1; test_index >= 0; test_index--)  {
        for (unsigned i = 0; i < KEYS_AMOUNT; i++) {
            keys[i] = rand () * rand();
        }
        time[test_index] = 0;
        found = 0;
        unsigned nodes = (TEST_AMOUNT - test_index) * NODES_AMOUNT;
        for (int i = 0; i < NODES_AMOUNT; i++) {
            set_tree(tree, rand () * rand(), 0);
        }
        for (int jndex = 0; jndex < N; jndex++) {
            gettimeofday(&first, NULL);
            for (unsigned i = 0; i < KEYS_AMOUNT; i++) {
                search_by_key(tree, keys[i]);
                found += find_last_release(tree, keys[i]);
            }
            gettimeofday(&last, NULL);
            time[test_index]  += (double) (last.tv_usec - first.tv_usec) / 1000000 +  (double)   (last.tv_sec - first.tv_sec);;
        }
        time[test_index] /= N;
        printf("test #%d, number of nodes = %u, time = %lf\n", TEST_AMOUNT - test_index, nodes, time[test_index]);
        printf("%u times keys were founded\n", found);
    }
    delete_tree(tree);
    return msg;
    }

ErrTree timing_in_order(double * const time) {
    srand(rand());
    BinaryTree * tree = create_tree();
    if (tree == NULL) {
        return ERR_TREE_MEM;
    }
    printf("\nIN ORDER TRANSVERSAL TEST\n");
    struct timeval first, last;
    ErrTree msg = ERR_TREE_OK;
    for (int test_index = TEST_AMOUNT - 1; test_index >= 0; test_index--)  {
        time[test_index] = 0;
        unsigned nodes = (TEST_AMOUNT - test_index) * NODES_AMOUNT;
        for (int i = 0; i < NODES_AMOUNT; i++) {
            set_tree(tree, rand () * rand(), 0);
        }
        gettimeofday(&first, NULL);
        Vect * pvect = in_order(tree->root);
        gettimeofday(&last, NULL);
        delete_vect(pvect);
        time[test_index]  += (double) (last.tv_usec - first.tv_usec) / 1000000 +  (double)   (last.tv_sec - first.tv_sec);;

        time[test_index] /= N;
        printf("test #%d, number of nodes = %u, time = %lf\n", TEST_AMOUNT - test_index, nodes, time[test_index]);
    }
    end:
    delete_tree(tree);
    return msg;
}

ErrTree timing_delete(double * const time) {
    srand(rand());
    BinaryTree * tree = create_tree();
    if (tree == NULL) {
        return ERR_TREE_MEM;
    }
      printf("\nDELETE TEST\n");
    struct timeval first, last;
    ErrTree msg = ERR_TREE_OK;
    unsigned deleted[TEST_AMOUNT] = {0};
    for (int jndex = 0; jndex < N; jndex++) {
        memset_tree(tree);
       for (int test_index = TEST_AMOUNT - 1; test_index >= 0; test_index--)  {
           for (int i = 0; i < NODES_AMOUNT; i++) {
               set_tree(tree, rand () * rand(), 0);
           }
            gettimeofday(&first, NULL);
            for (unsigned i = 0; i < KEYS_AMOUNT; i++) {
                if (remove_by_key(tree, rand() * rand()) == ERR_TREE_OK) {
                    deleted[test_index]++;
                }
            }
            gettimeofday(&last, NULL);
            time[test_index]  += (double) (last.tv_usec - first.tv_usec) / 1000000 +  (double)   (last.tv_sec - first.tv_sec);;
        }
    }
    for (int test_index = TEST_AMOUNT - 1; test_index >= 0; test_index--) {
        time[test_index] /= N;
        printf("test #%d, number of nodes = %u, time = %lf\n", TEST_AMOUNT - test_index,
               (TEST_AMOUNT - test_index) * NODES_AMOUNT, time[test_index]);
        printf("%u timed keys were removed\n", deleted[ TEST_AMOUNT - test_index - 1]);
    }

    delete_tree(tree);
    return msg;
}

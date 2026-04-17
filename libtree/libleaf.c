#include <stdio.h>
#include <stdlib.h>
#include "bsttree.h"
#include "internal.h"
#define COLOR_PURPLE "\x1B[35m"
#define COLOR_GREEN  "\x1B[32m"
#define COLOR_RESET  "\x1B[0m"

Node * create_leaf(void) {
    return (Node*) calloc(1, sizeof(Node));
}

void memset_leaf(Node * pleaf) {
    if (pleaf == NULL) {
        return;
    }
    free(pleaf->info);
    pleaf->info = NULL;
    pleaf->left = NULL;
    pleaf->parent = NULL;
    pleaf->key = 0;
}

ErrTree set_relations(Node* const parent, Node* const child) {
    if (parent == NULL || child == NULL) {
        return ERR_TREE_LEAF_EMPTY;
    }
    if (parent->key <= child->key && parent->right == NULL) {
        parent->right = child;
        return ERR_TREE_OK;
    }
    if (parent->key > child->key && parent->left == NULL) {
        parent->left = child;
        return ERR_TREE_OK;
    }
    return ERR_TREE_RELATIONS;
}

ErrTree set_leaf(Node* const leaf, const unsigned key, const unsigned info) {
    leaf->info = calloc(1, sizeof(unsigned));
    if (leaf->info == NULL) {
        return ERR_TREE_MEM;
    }
    *leaf->info = info;
    leaf->key = key;
    leaf->next = NULL;
    return ERR_TREE_OK;
}

Node * copy_leaf(const Node* const original) {
    Node * leaf;
    leaf->info = calloc(1, sizeof(unsigned));
    if (leaf->info == NULL) {
        return NULL;
    }
    *leaf->info = *original->info;
    leaf->key = original->key;
    return leaf;
}
void print_table_leaf_header(void) {
    printf("+--------------+--------------+\n");
    printf("|     Key      |    Info      |\n");
    printf("+--------------+--------------+\n");
}
void print_table_leaf_footer(void) {
    printf("+--------------+--------------+\n");
}
void print_leaf(const Node* const leaf) {
    if (leaf == NULL) {
        printf("| %-14s | %-14s |\n", "NULL", "NULL");
        return;
    }
    for (Node * q = leaf; q != NULL; q = q->next) {
        printf("| " COLOR_PURPLE "%-12u" COLOR_RESET " | ", q->key);
        printf(COLOR_GREEN "%-12u" COLOR_RESET " |\n", q->info == NULL ? 0 : *q->info);
    }
}
Node * left_most(Node * n) {
    if (n == NULL) {
        return NULL;
    }
    while (n->left != NULL) {
        n = n->left;
    }
    return n;
}

void print_node(const Node* node) {
     printf("\n");
    if (node == NULL) {
        printf("┌───────────────┐\n");
        printf("│   Node: NULL  │\n");
        printf("└───────────────┘\n");
        return;
    }
    printf("┌───────────────────┐\n");
    printf("│ node_%-12p │\n", (void*)node);
    printf("├───────────────────┤\n");
    printf("│ KEY: %-12u │\n", node->key);
    printf("│ L: %-14p │\n", (void*)node->left);
    printf("│ R: %-14p │\n", (void*)node->right);
    printf("│ P: %-14p │\n", (void*)node->parent);
    printf("└───────────────────┘\n");
}

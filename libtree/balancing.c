#include "bsttree.h"
#include "vect.h"
#define ALPHA 2.0/3
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "internal.h"

bool is_scapegoat(Node* const parent) {
    if (parent == NULL) {
        return true;
    }
    double size_parent = find_size_branch(parent);
    if (size_parent == 0) {
        return false;
    }
    double size_child = find_size_branch(parent->left);
    if (size_child / size_parent - ALPHA > 0.0001 ) {
        DEBUG_PRINT("key %u is scapegoat\n", parent->key);
        return true;
    }
    size_child = find_size_branch(parent->right);
     if (size_child / size_parent - ALPHA > 0.0001 ) {
         DEBUG_PRINT("key %u is scapegoat\n", parent->key);
         return true;
    }
    return false;
}
unsigned find_size_branch(Node * pnode) {
    Vect * pv = in_order(pnode);
    if (pv == NULL) {
        return 0;
    }
    unsigned size = pv->cap;
    delete_vect(pv);
    return size;
}

bool is_balanced(BinaryTree * const tree, Node * const elem) {
    if (tree == NULL || elem == NULL) {
        return true;
    }
    if (tree->nsize < 5) {
        return true;
    }
    unsigned elem_depth = find_branch_depth(tree, elem);
    unsigned max_allowed_depth = (unsigned) (log(tree->nsize) / log(1.5));
    DEBUG_PRINT("Node depth: %u, Max allowed: %u\n", elem_depth, max_allowed_depth);
    return (elem_depth <= max_allowed_depth);
}

ErrTree rebuilding_tree(BinaryTree * const tree, Node * const elem) {
    if (elem == NULL) {
        return ERR_TREE_KEY_NOT_EXIST;
    }
    Vect * pvect = in_order(elem);
    if (pvect == NULL) {
        return ERR_TREE_MEM;
    }
    if (pvect->cap <= 3) {
        delete_vect(pvect);
        return ERR_TREE_OK;
    }
    //Отделяем ветвь
    if (elem->parent == NULL) {
        tree->root = NULL;
    } else {
        if (elem->parent->right == elem) {
            elem->parent->right = NULL;
        } else {
            elem->parent->left = NULL;
        }
    }
    tree->msize -= pvect->cap;
    tree->nsize -= pvect->cap;
    BinaryTree subtree;
    subtree.root = elem;
    subtree.nsize = subtree.msize = 0;
    elem->parent = NULL;
    memset_tree(&subtree);



    Node * data = pvect->data;
    for (Node * q = data + pvect->cap/2; q != NULL; q = q->next) {
        set_tree(tree, q->key, *q->info);
    }
    for (int i = pvect->cap / 2 - 2; i > 0; i -= 2) {
        for (Node * q = data + i; q != NULL; q = q->next) {
            set_tree(tree, q->key, *q->info);
        }
    }
    for (int i = pvect->cap / 2 + 2; i < pvect->cap; i += 2) {
        for (Node * q = data + i; q != NULL; q = q->next) {
            set_tree(tree, q->key, *q->info);
        }
    }
    int i = 0;
    for (i = pvect->cap / 2 - 1; i > 2; i -= 2) {};
    for (; i < pvect->cap; i += 2) {
        for (Node * q = data + i; q != NULL; q = q->next) {
            set_tree(tree, q->key, *q->info);
        }
    };
    for (Node * q = data; q != NULL; q = q->next) {
        set_tree(tree, q->key, *q->info);
    }
    delete_vect(pvect);
    return ERR_TREE_OK;
}

unsigned find_branch_depth(BinaryTree * const tree, Node * const elem) {
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }
    unsigned depth = 0;
    for (Node* q = tree->root; q != NULL; depth++) {
        if (q->key == elem->key && q->info == elem->info) {
            break;
        }
        q = (elem->key < q->key) ? q->left : q->right;
    }
    return depth;
}

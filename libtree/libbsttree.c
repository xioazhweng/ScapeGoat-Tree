#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "bsttree.h"
#include "stack.h"
#include "vect.h"
#include "internal.h"
#include "../lib/input.h"


BinaryTree * create_tree (void) {
    return calloc(1, sizeof(BinaryTree ));
}

void delete_tree(BinaryTree * tree) {
    if (tree == NULL) {
        return;
    }
    tree->msize = 0;
    tree->nsize = 0;
    while (tree->root != NULL) {
        remove_by_key(tree, tree->root->key);
    }
    free(tree);
}

void memset_tree(BinaryTree * tree) {
    if (tree == NULL) {
        return;
    }
    while (tree->root != NULL) {
        remove_by_key(tree, tree->root->key);
    }
}

ErrTree set_tree(BinaryTree* tree,  const unsigned key, const unsigned info) {
    if (tree == NULL) {
        return ERR_TREE_NOT_INIT;
    }
    Node* ptr, * parent;
    ptr = parent = NULL;
    if (tree->root == NULL) {
        tree->nsize = 0;
        tree->msize = 0;
    } else {
        ptr = tree->root;
        while (ptr != NULL) {
            parent = ptr;
            if (key < ptr->key) {
                ptr = ptr->left;
            } else if (key != ptr->key){
                ptr = ptr->right;
            } else {
                break;
            }
        }
    }
    DEBUG_PRINT("ptr is %p\n", ptr);
    Node * new_elem = create_leaf();
    if (new_elem == NULL) {
        return ERR_TREE_MEM;
    }
    new_elem->left = NULL;
    new_elem->right = NULL;
    new_elem->next = NULL;
    new_elem->parent = parent;
    if (set_leaf(new_elem, key, info) != ERR_TREE_OK) {
        free(new_elem);
        return ERR_TREE_MEM;
    }
    if (ptr == NULL) {   //новый элемент в дереве
        tree->nsize++;
        tree->msize++;
        if (tree->root == NULL) {
            tree->root = new_elem;
        } else {
            set_relations(parent, new_elem);
        }
        if (!is_balanced(tree, new_elem)) {
            parent = new_elem->parent;
            while (!is_scapegoat(parent)) {
                parent = parent->parent;
            }
            rebuilding_tree(tree, parent);
        }
    } else {   //дублирование ключа
        new_elem->right = ptr->right;
        new_elem->left = ptr->left;
        if (ptr->right != NULL) {
            ptr->right->parent = new_elem;
        }
        if (ptr->left != NULL) {
            ptr->left->parent = new_elem;
        }
        if (tree->root == ptr) {
            tree->root = new_elem;
        } else {
            if (ptr->parent->left == ptr) {
                ptr->parent->left = new_elem;
            } else {
                ptr->parent->right = new_elem;
            }
        }
        ptr->right = ptr->left = NULL;
        new_elem->parent = ptr->parent;
        new_elem->next = ptr;
        }
    return ERR_TREE_OK;
}

Node * search_by_key(const BinaryTree* const tree, const unsigned key) {
    if (tree == NULL) {
        return NULL;
    }
    Node * q = NULL;
    for (q = tree->root; q != NULL;) {
        if (q->key == key) {
            break;
        }
        q = (key < q->key) ? q->left : q->right;
    }
    return q;
}

unsigned find_last_release(const BinaryTree* const tree, const unsigned key) {
    if (tree == NULL) {
        return 0;
    }
    Node * q = search_by_key(tree, key);
    if (q == NULL) {
        return 0;
    }
    unsigned release = 0;
    for (; q != NULL; release++, q = q->next) {};
    return release;
}

void  print_in_order_more_than_key(BinaryTree* const tree, const unsigned min_key) {
    Vect * pvect = in_order(tree->root);
    Node * data = pvect->data;
    print_table_leaf_header();
    for (int i = 0; i < pvect->cap; i++) {
        if (data[i].key < min_key) {
            continue;
        }
        print_leaf(data + i);
    }
    print_table_leaf_footer();
    delete_vect(pvect);
}
Node * search_max_less_key(const BinaryTree* const tree, const unsigned max_key) {
    if (tree->root == NULL) {
        return NULL;
    }
    Node * p;
    for (p = tree->root; p->left != NULL &&  p->key > max_key ; p = p->left) {};
    for (; p->right != NULL; p = p->right) {
        if (p->right->key > max_key) {
            break;
        }
    }
    if (p->key > max_key) {
        return NULL;
    }
    return search_by_key(tree, p->key);
}

Vect* search_neihgbours_key(const BinaryTree* const tree, const unsigned key) {
    Vect * in_order_vect = in_order(tree->root);
    if (in_order_vect == NULL) {
        return NULL;
    }
    Vect * pvect = create_vect(sizeof(Node));
    if (pvect == NULL) {
        return NULL;
    }
    Node * data = in_order_vect->data;
    int index;
    for (index = 0; index < in_order_vect->cap; index++) {
        if (data[index].key >= key) {
            break;
        }
    }
    if (index != 0) {
        append_vect(pvect, data + index - 1);
    }
    if (index != in_order_vect->cap - 1) {
        append_vect(pvect, data + index + 1);
    }
    delete_vect(in_order_vect);
    return pvect;
}

void printf_tree(const BinaryTree* const ptree) {
    if (ptree == NULL || ptree->root == NULL) {
        printf("Empty tree\n");
        return;
    }
    printf("Unique keys(actual): %u\n", ptree->nsize);
    printf("Unique keys(added): %u\n", ptree->msize);
    Stack * pstack = create_stack();
    StackNode current;
    current.leaf = ptree->root;
    current.level = 0;
    current.next = NULL;

    while (1) {
        if (current.level > 0) {
            for (int i = 0; i < current.level - 1; i++) {
                printf("│ ");
            }
            if (current.leaf->parent->right != NULL ) {
                printf("├─");
            } else {
                printf("└─");
            }
        }
        if (current.leaf != NULL) {
            for(Node * q = current.leaf; q != NULL; q = q->next) {
                printf("%u-", q->key);
            }
            printf("\n");

            if (current.leaf->right != NULL) {
                push_stack(pstack, current.leaf->right, current.level + 1);
            }
            if (current.leaf->left != NULL) {
                current.leaf = current.leaf->left;
                current.level++;
                continue;
            }
        } else {
            printf("\n");
        }
        if (!is_empty_stack(pstack)) {
            pop_stack(pstack, &current);
        } else {
            break;
        }
    }
    delete_stack(pstack);
}

ErrTree remove_by_key(BinaryTree* const tree, const unsigned key) {
    if (tree == NULL) {
        return ERR_TREE_NOT_INIT;
    }
    if (tree->root == NULL) {
        return ERR_TREE_EMPTY;
    }
    Node * q = search_by_key(tree, key);
    if (q == NULL) {
        return ERR_TREE_KEY_NOT_EXIST;
    }
    DEBUG_PRINT("%u\n", q->key);
    Node * next, *p;
    p = q->next;
    q->next = NULL;
    while (p != NULL) {
        next = p->next;
        free(p->info);
        free(p);
        p = next;
    }
    return remove_tree_node(tree, key);
}

ErrTree remove_by_key_and_release(BinaryTree* const tree, const unsigned key, const unsigned release) {
    if (tree == NULL) {
        return ERR_TREE_NOT_INIT;
    }
    if (tree->root == NULL) {
        return ERR_TREE_EMPTY;
    }
    Node * q = search_by_key(tree, key);
    if (q == NULL) {
        return ERR_TREE_KEY_NOT_EXIST;
    }
    if (q->next == NULL && release != 1) {
        return ERR_TREE_KEY_NOT_EXIST;
    } else if (q->next == NULL) {
        return remove_tree_node(tree, key);
    }
    //удаление в списке
    if (release == 1) {
        q->next->parent = q->parent;
        q->next->left = q->left;
        q->next->right = q->right;
        if (tree->root == q) {
            tree->root = q->next;

        } else {
            if (q->parent->left == q) {
                q->parent->left = q->next;
            } else {
                q->parent->right = q->next;
            }
            free(q->info);
            free(q);
        }
        return ERR_TREE_OK;
    }
    Node * prev;
    for(int i = 1; i < release; i++) {
        prev = q;
        q = q->next;
        if (q == NULL) {
            return ERR_TREE_KEY_NOT_EXIST;
        }
    }
    prev->next = q->next;
    free(q->info);
    free(q);
    return ERR_TREE_OK;
}

ErrTree remove_tree_node(BinaryTree* const tree, const unsigned key) {
    if (tree == NULL) {
        return ERR_TREE_NOT_INIT;
    }
    if (tree->root == NULL) {
        return ERR_TREE_EMPTY;
    }
    Node * ptr = tree->root;
    Node * elem = search_by_key(tree, key);
    if (elem == NULL) {
        return ERR_TREE_KEY_NOT_EXIST;
    }
    // Случай 1. Никаких детей
    if (elem->left == NULL && elem->right == NULL) {
        if (elem->parent == NULL) {     //удаление корня
            tree->root = NULL;
        } else {
            if (elem->parent->left == elem) {
                elem->parent->left = NULL;
            } else {
                elem->parent->right = NULL;
            }
        }
        free(elem->info);
        free(elem);
    }
     // Случай 2. Два ребенка
    else if (elem->left != NULL && elem->right != NULL) {
        Node * prey = left_most(elem->right);
        if (prey == elem->right) {
            elem->right = prey->right;
            if (prey->right != NULL) {
                prey->right->parent = elem;
            }
        } else {
            prey->parent->left = prey->right;
            if (prey->right != NULL) {
                prey->right->parent = prey->parent;
            }
        }
        free(elem->info);
        elem->info = prey->info;
        elem->key = prey->key;
        elem->next = prey->next;
        free(prey);
    }
    // Случай 3. Один ребенок
    else  {
        Node * prey = (elem->left != NULL) ? elem->left : elem->right;
        if (elem->parent == NULL) { //корень
           tree->root  = prey;
            prey->parent = NULL;
        } else {
            if (elem->parent->left == elem) {
                elem->parent->left = prey;
            } else {
                elem->parent->right = prey;
            }
            prey->parent = elem->parent;
        }
        free(elem->info);
        free(elem);
    }
    if (tree->nsize) {
        tree->nsize--;
    }

    if (tree->msize > 2* tree->nsize) {
        rebuilding_tree(tree, tree->root);
    }

    return ERR_TREE_OK;
}

void err_tree_message(ErrTree request) {
    switch (request) {
        case ERR_TREE_OK:
            break;
        case ERR_TREE_NOT_INIT:
            fprintf(stderr, "Tree is not initializated\n");
            break;
        case ERR_TREE_EMPTY:
            fprintf(stderr,"Tree is empty\n");
            break;
        case ERR_TREE_MEM:
            fprintf(stderr, "Couldn't allocate memory\n");
            break;
        case ERR_TREE_UPDATE:
            fprintf(stderr, "This feature will be released soon. Wait for the update ˙▿˙ \n");
             break;
        case ERR_TREE_KEY_NOT_EXIST:
            fprintf(stderr,"The key doesn't exist\n");
            break;
        case ERR_TREE_LEAF_EMPTY:
            fprintf(stderr,"Node is not init\n");
            break;

        default:
            fprintf(stderr,"Unknown error\n");
    }
}

ErrTree import_tree(BinaryTree * tree, const char* filename) {
    if (tree == NULL) {
        return ERR_TREE_NOT_INIT;
    }
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return ERR_TREE_FILE_FAIL;
    }
    struct Data {
        unsigned key;
        unsigned info;
    } data;
    unsigned key, info;
    while (fread( &data, sizeof(data), 1, file) == 1) {
        set_tree(tree, data.key, data.info);
    }
    fclose(file);
    return ERR_TREE_OK;
}

ErrTree export_tree_txt(BinaryTree * const tree, const char* filename) {
    FILE * file = fopen(filename, "w");
    if (file == NULL && errno == EEXIST) {
        fprintf(stderr, "The file %s exists. Rewrite? [Y/n]\n", filename);
        if (read_yn_input() == ERR_OK) {
            file = fopen(filename, "w");
        }
    }
    if (file == NULL) {
        return ERR_TREE_FILE_FAIL;
    }
    Vect * pv = pre_order(tree->root);
    if (pv == NULL) {
        fclose(file);
        return ERR_TREE_MEM;
    }
    Node * arr = pv->data;
    for (int i = 0; i < pv->cap; i++) {
        for(Node* q = arr + i; q != NULL; q = q->next) {
            fprintf(file, "%10u%10u\n", q->key, *(q->info));
        }
    }
    delete_vect(pv);
    fclose(file);
    return ERR_TREE_OK;
}

ErrTree export_tree(BinaryTree * const tree, const char* filename) {
    if (tree == NULL) {
        return ERR_TREE_NOT_INIT;
    }
    ErrTree msg = ERR_TREE_OK;
    FILE * file = fopen(filename, "wb");
    if (file == NULL) {
        return ERR_TREE_FILE_FAIL;
    }
    Vect * pv = pre_order(tree->root);
    if (pv == NULL) {
        fclose(file);
        return ERR_TREE_MEM;
    }
    struct Data {
        unsigned key;
        unsigned info;
    } data;
    Node * arr = pv->data;
    for (int i = 0; i < pv->cap; i++) {
        for(Node* q = arr + i; q != NULL; q = q->next) {
            data.key = q->key;
            data.info = *(q->info);
            if (fwrite( &data, sizeof(data), 1, file) != 1) {
                msg = ERR_TREE_FILE_FAIL;
                goto end;
            }
        }
    }
    end:
    fclose(file);
    delete_vect(pv);
    return msg;
}

Vect * in_order(Node * const root) {
    if (root == NULL) {
        return NULL;
    }
    Vect * pvect = create_vect(sizeof(Node));
    if (pvect == NULL) {
        return NULL;
    }
    Stack * pstack = create_stack();
    if (pstack == NULL) {
        delete_vect(pvect);
        return NULL;
    }
    Node * ptr = root;
    StackNode m;
    Node * prev = NULL;
    do {
        while (ptr != NULL) {
            push_stack(pstack, ptr, 0);
            ptr = ptr->left;
        }
        pop_stack(pstack, &m);
        ptr = m.leaf;
        append_vect(pvect, ptr);
        ptr = ptr->right;

    } while (!is_empty_stack(pstack) || ptr != NULL);
    delete_stack(pstack);
    return pvect;
}

Vect * pre_order(Node * const root) {
    if (root == NULL) {
        return NULL;
    }
    Vect * pvect = create_vect(sizeof(Node));
    if (pvect == NULL) {
        return NULL;
    }
    Stack * pstack = create_stack();
    if (pstack == NULL) {
        delete_vect(pvect);
        return NULL;
    }
    StackNode m;
    Node * ptr = root;
    while(1) {
        if (ptr != NULL) {
            append_vect(pvect, ptr);
            if (ptr->right != NULL) {
                push_stack(pstack, ptr->right, 0);
            }
            if (ptr->left != NULL) {
                append_vect(pvect, ptr);
                ptr = ptr->left;
                continue;
            }
        }
        if (!is_empty_stack(pstack)) {
            pop_stack(pstack, &m);
            ptr = m.leaf;
        } else {
            break;
        }
    }
    delete_stack(pstack);
    return pvect;
}

ErrTree generate_tree(BinaryTree * const tree, const unsigned amount_of_keys,
                      const unsigned max_key, const unsigned min_key) {
    ErrTree msg = ERR_TREE_OK;
    memset_tree(tree);
    srand(time(NULL));
    for (unsigned i = 0; i < amount_of_keys; i++) {
        unsigned key =  min_key + ((rand() * rand()) % (max_key - min_key + 1));
        msg = set_tree(tree, key, rand() % 2000);
        if ( msg != ERR_TREE_OK) {
            break;
        }
    }
    if (msg != ERR_TREE_OK) {
        memset_tree(tree);
    }
    return msg;
}


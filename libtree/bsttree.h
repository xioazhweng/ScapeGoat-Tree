#ifndef BSTTREE_H
#define BSTTREE_H
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

typedef struct Vect Vect;
typedef struct Node Node;
struct Node {
    unsigned key;
    unsigned* info;
    Node * parent, * left, * right;
    Node * next;
};

typedef struct BinaryTree {
    Node * root;
    unsigned nsize; //всего элементов
    unsigned msize; //добавленые элементы
                    //msize-nsize удаленные элементы
} BinaryTree;

typedef enum ErrTree {
    ERR_TREE_EOF = -1,
    ERR_TREE_OK = 0,
    ERR_TREE_MEM,
    ERR_TREE_EMPTY,
    ERR_TREE_KEY_NOT_EXIST,
    ERR_TREE_UPDATE,
    ERR_TREE_LEAF_EMPTY,
    ERR_TREE_RELATIONS,
    ERR_TREE_NOT_INIT,
    ERR_TREE_INFO_NOT_INIT,
    ERR_TREE_FILE_FAIL,
} ErrTree;


BinaryTree* create_tree (void);
void        delete_tree(BinaryTree * tree);
ErrTree     set_tree(BinaryTree* tree,  const unsigned key, const unsigned info);
void        memset_tree(BinaryTree * tree);
Node*       search_by_key(const BinaryTree* const tree, const unsigned key);
void        print_in_order_more_than_key(BinaryTree* const tree, const unsigned max_key);

Node*       search_max_less_key(const BinaryTree* const tree, const unsigned key);
Vect*       search_neihgbours_key(const BinaryTree* const tree, const unsigned key);
void        printf_tree(const BinaryTree* const ptree);
ErrTree     remove_by_key(BinaryTree* const tree, const unsigned key);
ErrTree     remove_by_key_and_release(BinaryTree* const tree, const unsigned key,
                                      const unsigned release);
void        err_tree_message(const ErrTree request);
ErrTree     import_tree(BinaryTree* const tree, const char* const filename);
ErrTree     export_tree(BinaryTree * tree, const char* filename);
ErrTree     export_tree_txt(BinaryTree * tree, const char* filename);
ErrTree     generate_tree(BinaryTree * const tree, const unsigned amount_of_keys,
                          const unsigned max_key, const unsigned min_key);
ErrTree     timing_tree(const char * filename);

//GRAPH FUNC
ErrTree draw(BinaryTree * const tree);
ErrTree add_leafs_to_graph(Agraph_t * g, BinaryTree  * tree, GVC_t * gvc);



#endif

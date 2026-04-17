#ifndef INTERNAL_H
#define INTERNAL_H

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG: %s:%d%s(): " fmt, \
__FILE__, __LINE__, __func__, ##args)
#else
#define DEBUG_PRINT(fmt, args...) ((void) 0)
#endif //DEBUG

typedef struct Vect Vect;
typedef struct Node Node;
typedef struct BinaryTree BinaryTree;
typedef enum ErrTree ErrTree;


ErrTree  remove_tree_node(BinaryTree* const, const unsigned );
unsigned find_size_branch(Node* const);
bool     is_scapegoat(Node* const);
ErrTree  rebuilding_tree(BinaryTree * const, Node* const);
bool     is_balanced(BinaryTree * const, Node* const);
Vect*    pre_order(Node* const);
Vect*    in_order(Node* const);
unsigned find_last_release(const BinaryTree* const, const unsigned);
unsigned find_branch_depth(BinaryTree * const tree, Node * const elem);
//LEAF FUNCTIONS
void    print_leaf(const Node* const);
void    print_table_leaf_header(void);
void    print_table_leaf_footer(void);
Node*   create_leaf(void);
Node *  copy_leaf(const Node* const);
void    memset_leaf(Node *);
ErrTree set_relations(Node* const, Node* const);
ErrTree set_leaf(Node* const, const unsigned, const unsigned);
Node*   search_min(Node* const);
Node *  left_most(Node * n);
void print_node(const Node* node);
#endif

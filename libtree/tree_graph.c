#include <stdlib.h>
#include <stdio.h>
#include "bsttree.h"
#include "stack.h"
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

#define WORD_SIZE 100
#define INF ((unsigned int)(−1))

void get_label(char * label, Node * qleaf);

ErrTree add_leafs_to_graph(Agraph_t * g, BinaryTree  * tree, GVC_t * gvc) {
    if (tree == NULL) {
        return ERR_TREE_NOT_INIT;
    }
    if (tree->root == NULL) {
        return ERR_TREE_EMPTY;
    }
    Stack * pstack = create_stack();
    StackNode current;
    current.leaf = tree->root;
    current.next = NULL;

    agsafeset(g, "ordering", "out", "");

    while (1) {
        char node_name[WORD_SIZE];
        sprintf(node_name, "node_%p", (void *) current.leaf);
        Agnode_t * graph_node = agnode(g, node_name, 1);

        char label[5*WORD_SIZE];
        get_label(label, current.leaf);
        agsafeset(graph_node, "label", label, "");
        agsafeset(graph_node, "shape", "box", "");


        if (current.leaf != NULL && current.leaf->parent != NULL) {
            char parent_name[WORD_SIZE];
            sprintf(parent_name, "node_%p", (void*) current.leaf->parent);
            Agnode_t * parent_graph_node = agnode(g, parent_name, 0);

            if (parent_graph_node != NULL) {
                Agedge_t *edge = agedge(g, parent_graph_node, graph_node, NULL, 1);
                if (current.leaf->parent->left == current.leaf) {
                    agsafeset(edge, "color", "red", "");
                } else {
                    agsafeset(edge, "color", "blue", "");
                }
                agsafeset(edge, "arrowhead", "normal", "");
            }
        }


        Node* next_node = current.leaf->next;
        Agnode_t* prev_graph_node = graph_node;

        while (next_node != NULL) {
            char next_name[WORD_SIZE];
            sprintf(next_name, "node_%p", (void*)next_node);
            Agnode_t* next_graph_node = agnode(g, next_name, 1);
            if (next_graph_node != NULL) {
                char next_label[5*WORD_SIZE];
                get_label(next_label, next_node);
                agsafeset(next_graph_node, "label", next_label, "");
                agsafeset(next_graph_node, "shape", "box", "");
                Agedge_t* next_edge = agedge(g, prev_graph_node, next_graph_node, NULL, 1);
                agsafeset(next_edge, "style", "dotted", "");
                agsafeset(next_edge, "color", "black", "");
                agsafeset(next_edge, "arrowhead", "none", "");
                prev_graph_node = next_graph_node;
            }
            next_node = next_node->next;
        }


        if (current.leaf->right != NULL) {
            push_stack(pstack, current.leaf->right, current.level + 1);
        }
        if (current.leaf->left != NULL) {
            current.leaf = current.leaf->left;
            current.level++;
            continue;
        }
        if (!is_empty_stack(pstack)) {
            pop_stack(pstack, &current);
        } else {
            break;
        }
    }
    delete_stack(pstack);
    return ERR_TREE_OK;
}

ErrTree draw(BinaryTree * const tree) {
    GVC_t *gvc = gvContext();
    Agraph_t *g = agopen("BinaryTree", Agstrictdirected, NULL);

    agsafeset(g, "rankdir", "TB", ""); // сверху вниз
    agsafeset(g, "nodesep", "0.5", "");
    agsafeset(g, "ranksep", "0.8", "");

    ErrTree mes = add_leafs_to_graph(g, tree, gvc);
    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, "pdf", fopen("tree.pdf", "wb"));
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    return mes;
}
#ifdef GRAPH_FULL
void get_label(char * label, Node * qleaf) {
    sprintf(label, "node_%p\nKEY: %u\nINFO: %u\n L: %p\nR: %p\nP: %p\n",
            (void *) qleaf, qleaf->key, *(qleaf->info),
            (void *) qleaf->left, (void *) qleaf->right, (void *) qleaf->parent);
}
#else
void get_label(char * label, Node * qleaf) {
    sprintf(label, "KEY: %u\nINFO: %u\n", qleaf->key, *(qleaf->info));
}
#endif

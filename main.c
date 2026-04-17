#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include "libtree/bsttree.h"
#include "lib/input.h"
#include "libtree/vect.h"
#include "libtree/internal.h"

#define TRY(act, label) if(!(act)) {goto label;}
#define MENU_AMOUNT 12

void dialog(BinaryTree* const tree);

ErrTree display_menu(BinaryTree* const tree);
ErrTree D_insert_tree(BinaryTree* const tree);
ErrTree D_remove_tree(BinaryTree* const tree);
ErrTree D_search_tree(BinaryTree* const tree);
ErrTree D_search_eps_tree(BinaryTree* const tree);
ErrTree D_print_tree(BinaryTree* const tree);
ErrTree D_import_tree(BinaryTree* const tree);
ErrTree D_export_tree(BinaryTree* const tree);
ErrTree D_print_tree_in_order(BinaryTree* const tree);
ErrTree D_timing_tree(BinaryTree* const tree);
ErrTree D_draw_graph(BinaryTree* const tree);
ErrTree D_generate_tree(BinaryTree* const tree);
ErrTree (*fptr[])(BinaryTree* tree) = {
    display_menu,
    D_insert_tree,
    D_remove_tree,
    D_print_tree,
    D_search_tree,
    D_import_tree,
    D_export_tree,
    D_print_tree_in_order,
    D_search_eps_tree,
    D_timing_tree,
    D_draw_graph,
    D_generate_tree,
};

int main(void) {
    BinaryTree * tree = create_tree();
    if (tree == NULL) {
        err_tree_message(ERR_TREE_MEM);
        return 1;
    }
    dialog(tree);
    delete_tree(tree);
    printf("\n");
    return 0;
}

void dialog(BinaryTree* const tree) {
    unsigned int answer;
    ErrTree msg;
    display_menu(NULL);
    printf(">  ");
    while (get_uint(&answer,  NOT_NEG) != EOF) {
        if (answer <  MENU_AMOUNT) {
            msg = fptr[answer](tree);
            if (msg == ERR_TREE_EOF) {
                break;
            }
            err_tree_message(msg);
            printf(">  ");
        }
        else {
            printf("Noting to do\n");
        }
    }
}

ErrTree display_menu(BinaryTree* const tree) {
    const char * menu[] = {
        " 0. To see menu",
        " 1. Insert element with key",
        " 2. Delete element by key",
        " 3. Display tree content",
        " 4. Search element by key",
        " 5. Import from file",
        " 6. Export to file",
        " 7. Print in order",
        " 8. Search key's neighbours",
        " 9. Timing",
        "10. Cgraph fuction",
        "11. Generate tree",
        "<Ctrl + D>. Exit"
    };
    const unsigned Nmsgs = sizeof(menu) / sizeof(menu[0]);
    printf("\n=== BinaryTree Management Menu ===\n");
    for(unsigned i = 0; i < Nmsgs; i++) {
        puts(menu[i]);
    }
    return ERR_TREE_OK;
}

ErrTree D_insert_tree(BinaryTree* tree) {
    ErrTree msg = ERR_TREE_OK;
    unsigned info;
    unsigned key;
    printf("Input key: ");
    TRY(get_uint(&key, POSITIVE) != EOF, end)
    printf("Input info: ");
    TRY(get_uint(&info, NOT_NEG) != EOF, end)
    msg = set_tree(tree, key, info);
    end:
    return msg;
}

ErrTree D_remove_tree(BinaryTree* const tree) {
    unsigned key, release;
    ErrTree mes = ERR_TREE_OK;
    printf("Input key: ");
    TRY(get_uint(&key, POSITIVE) != EOF, end);
    unsigned releases = find_last_release(tree, key);
    if (releases != 1) {
        printf("Delete all elements?[Y/n] ");
        Err ans = read_yn_input();
        if (ans == EOF) {
            goto end;
        }
        if (ans != ERR_OK) {
            printf("Input sequence number: ");
            TRY(get_uint(&release, POSITIVE) != EOF, end);
            mes = remove_by_key_and_release(tree, key, release);
            goto end;
        }
    }
    mes = remove_by_key(tree, key);
    end:
    return mes;
}

ErrTree D_search_tree(BinaryTree* const tree) {
    ErrTree mes = ERR_TREE_OK;
    unsigned key;
    printf("Input key: ");
    TRY(get_uint(&key, POSITIVE) != EOF, end);

    Node * p = search_by_key(tree, key);
    if (p == NULL) {
        printf("Nothing was found\n");
    }
    print_table_leaf_header();
    print_leaf(p);
    print_table_leaf_footer();
    end:
    return ERR_TREE_OK;
}

ErrTree D_print_tree(BinaryTree* const tree) {
    printf_tree(tree);
    return ERR_TREE_OK;
}

ErrTree D_import_tree(BinaryTree* const tree) {
    char * str;
    str = readline("Input name of file: ");
    if (str == NULL) {
        return ERR_TREE_OK;
    }
    char * filename = strtok(str, " \t\n");
    ErrTree msg = import_tree(tree, filename);
    free(str);
    return msg;
}

ErrTree D_export_tree(BinaryTree* const tree) {
    char * str;
    str = readline("Input name of file: ");
    if (str == NULL) {
        return ERR_TREE_OK;
    }
    char * filename = strtok(str, " \t\n");
    ErrTree mes = export_tree(tree, filename);
    free(str);
    return mes;
}

ErrTree D_print_tree_in_order(BinaryTree* const tree) {
    unsigned key, release;
    printf("Input key: ");
    TRY(get_uint(&key, POSITIVE) != EOF, end);
    print_in_order_more_than_key(tree, key);
    end:
    return ERR_TREE_OK;
}

ErrTree D_search_eps_tree(BinaryTree* const tree) {
    unsigned key, release;
    printf("Input key: ");
    TRY(get_uint(&key, POSITIVE) != EOF, end);
    Vect * pv = search_neihgbours_key(tree, key);
    print_vect(pv);
    delete_vect(pv);
    end:
    return ERR_TREE_OK;
}

ErrTree D_timing_tree(BinaryTree* const tree) {
    srand(time(NULL));
    char * str;
    str = readline("Input name of file: ");
    if (str == NULL) {
        return ERR_TREE_OK;
    }
    char * filename = strtok(str, " \t\n");
    ErrTree msg = ERR_TREE_OK;
    msg = timing_tree(filename);
    free(str);
    return ERR_TREE_OK;
}

ErrTree D_draw_graph(BinaryTree* const tree) {
    return draw(tree);
}
ErrTree D_generate_tree(BinaryTree* const tree) {
    unsigned amount_keys;
    ErrTree msg = ERR_TREE_OK;
    printf("Input amount of keys: ");
    TRY(get_uint(&amount_keys, POSITIVE) != EOF, end);
    msg = generate_tree(tree, amount_keys, 200, 10);
    end:
    return msg;
}

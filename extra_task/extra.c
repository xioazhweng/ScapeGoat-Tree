#include <stdio.h>
#include <stdlib.h>
#include "../libtree/bsttree.h"
#include "../libtree/internal.h"
#include <string.h>
#include "../lib/input.h"
#include <time.h>
#include <errno.h>
#define TRY(act, label) if(!(act)) {goto label;}
#define MIN_KEY 100u
#define MAX_KEY 200u
#define DELIM " \t,"
#define _GNU_SOURCE
ErrTree import_tree_special(BinaryTree * tree, const char* filename);
ErrTree count_nums_in_file (BinaryTree * tree, FILE* f_from);
ErrTree generate_file (const char *file_name);
int main(int argc, char *argv[]) {
    ErrTree msg = ERR_TREE_OK;
    BinaryTree * ptree = NULL;

    if (argc != 2) {
        printf("Expected: ./extra input_file_name\n");
        return 1;
    }
    FILE * f_from;
    f_from = fopen(argv[1], "r");

    if (f_from == NULL && errno == ENOENT) {
        printf("Error: The file %s doesn't exist\n", argv[1]);
        printf("Generate new file? [Y/n]: ");
        if (read_yn_input() == ERR_OK) {
              msg = generate_file(argv[1]);
              if (msg != ERR_TREE_OK) {
                  goto end;
             }
             f_from = fopen(argv[1], "rb");
        }
    }
    if (f_from == NULL) {
        perror(argv[1]);
        return 1;
    }
    fclose(f_from);
    ptree = create_tree();
    if (ptree == NULL) {
        return ERR_TREE_MEM;
    }
    import_tree_special(ptree, argv[1]);
    unsigned key;
    do {
        printf("Input key: ");
        TRY(get_uint(&key, POSITIVE) != EOF, end);
        Node * p = search_by_key(ptree, key);
        if (p == NULL) {
            printf("This key does not exist\n");
        } else {
            print_table_leaf_header();
            print_leaf(p);
            print_table_leaf_footer();
        }
    } while(1);
    end:
    delete_tree(ptree);
    err_message(msg);
    return msg;
}

ErrTree generate_file (const char *file_name) {
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        return ERR_TREE_FILE_FAIL;
    }
    unsigned amount_of_keys = 0;
    printf("Input amount of keys: ");
    if (get_uint(&amount_of_keys, POSITIVE) == EOF) {
        fclose(file);
        return ERR_TREE_EOF;
    }
    srand(time(NULL));
    for (unsigned i = 0; i < amount_of_keys; i++) {
        fprintf(file, "%u,", MIN_KEY + ((rand() * rand()) % (MAX_KEY - MIN_KEY + 1)));
        if ((rand() % 30) % 2 == 0) {
            fprintf(file, "\n");
        }
    }
    fclose(file);
    return ERR_TREE_OK;
}

ErrTree import_tree_special(BinaryTree *tree, const char* filename) {
    if (tree == NULL) {
        return ERR_TREE_NOT_INIT;
    }
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return ERR_TREE_FILE_FAIL;
    }
    size_t len = 0;
    unsigned amount_lines = 0;
    unsigned long key;
    ssize_t read;
    char *num;
    char * line = NULL;
    char *endptr = NULL;
    while ((read = getline(&line, &len, file)) != -1) {
        amount_lines++;
        num = strtok(line, DELIM);
        while(num != NULL) {
            key = strtoul(num, &endptr, 10);
            if (key != 0 && key != -1) {
                set_tree(tree, key, amount_lines);
            }
            num = strtok(NULL, DELIM);
        }
    }
    free(line);
    fclose(file);
    return ERR_TREE_OK;
}

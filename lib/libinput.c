#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <ctype.h>
#include "input.h"

void err_message(const int request) {
    switch (request) {
        case ERR_MEMORY:
            printf("Couldn't allocate memory'\n");
            break;
        case ERR_SIZE:
            printf("Size should be positive. Nothing changed\n");
            break;
        case EOF:
            printf("\nForced termination of the program\n");
            break;
        case ERR_EXIST:
            printf("Object doesn't exist. Nothing changed\n");
            break;
        case ERR_OVERFLOW:
            printf("Reached the end of array. Nothing changed\n");
        case ERR_OK:
        default: break;
    }
}

Err get_int(int* const num) {
    int flag;
    do {
        flag = scanf("%d", num);
        if(flag < 0) {
            scanf("%*[^\n]");
            return EOF;
        } else if (flag == 0) {
            printf("Input should be integer. Try again\n");
            scanf("%*c");
        }
    } while (flag == 0);
    return ERR_OK;
}

Err get_uint(unsigned * const num, Mode mode) {
    int flag;
    do {
        flag = scanf("%u", num);
        if(flag < 0) {
            scanf("%*[^\n]");
            return EOF;
        } else if (flag == 0) {
            printf("Error: not unsigned integer. Try again\n>  ");
            scanf("%*[^\n]");
        }
        if (mode == POSITIVE && *num == 0) {
            printf("Value should be positive. Try again\n> ");
            scanf("%*[^\n]");
            flag = 0;
            }
    } while (flag == 0);
    return ERR_OK;
}


void reverse_string(char* const str) {
    unsigned int len = strlen(str);
    char x;
    for (int i = 0; i < len / 2; i++)
    {
        x = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = x;
    }
}
Err read_yn_input(void) {
    char yes[] = "yes";
    char * str;
    str = readline("");
    if (str == NULL) {
        return ERR_INPUT;
    }
    if (strlen(str) == 0) {
        free(str);
        return ERR_OK;
    }
    char * answer = strtok(str, " \t\n");
    for(unsigned i = 0; i <strlen(answer); i++) {
        answer[i] = tolower(answer[i]);
    }
    Err result = ERR_NOT_OK;
    if (answer[0] == 'y') {  // y, yes, yeah, etc.
        result = ERR_OK;
    } else if (answer[0] == 'n') {  // n, no
        result = ERR_NOT_OK;
    }

    free(str);
    return result;
}

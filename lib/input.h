#ifndef INPUT_H
#define INPUT_H

typedef enum Err {
    ERR_EOF = -1,
    ERR_OK = 0,
    ERR_NOT_OK = 1,
    ERR_SIZE = 2,
    ERR_EXIST = 3,
    ERR_MEMORY = 4,
    ERR_OVERFLOW = 5,
    ERR_UNDERFLOW = 6,
    ERR_INPUT,
} Err;

typedef enum Mode {
    POSITIVE,
    NOT_NEG,
    NEGATIVE,
} Mode;

void  err_message(const int request);
Err   get_int(int* const num);
Err   get_uint(unsigned int* const num, Mode mode);
void  reverse_string(char* const str);
Err read_yn_input(void);
#endif // INPUT_H

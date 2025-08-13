#include "include/parse.h"

size_t len(char* buff) {
    size_t t = 0;
    for (char* c = buff; *c!=0; c++) {
        t++;
    }
    return t;
}
int cut(char* buff, char* res, int start, size_t lenght) {
    if (buff == NULL || res == NULL) {
        return -1;
    }
    for (int i = 0; i < lenght; i++) {
        res[start+i] = buff[i];
    }
    return 0;
}
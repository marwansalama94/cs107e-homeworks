#include "printf.h"

int vsnprintf(char *buf, int bufsize, const char *format, va_list args){
    //TODO:to implement
}

int snprintf(char *buf, int bufsize, const char *format, ...) __attribute__ ((format (printf, 3, 4))){
    //TODO:to implement
}

int printf(const char *format, ...) __attribute__ ((format (printf, 1, 2))){
    //TODO:to implement
}

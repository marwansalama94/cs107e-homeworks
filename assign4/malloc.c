#include "malloc.h"

#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))
//gonna change to 16MB once finished testing
#define max_bytes 96

struct hdr{
    unsigned int payload_size;
    unsigned int status;
};

extern int __bss_end__;
static char* heap_start = &__bss_end__; //got to the end of the bss area



void *malloc(size_t nbytes){
    return 0;
}

void free(void *ptr){
    
}

void *realloc(void *ptr, size_t new_size){
    return 0;
}

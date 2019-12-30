#include "malloc.h"
#include "libs/printf.h"
#include "libs/timer.h"
#ifndef NULL
#define NULL 0
#endif


#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))
//gonna change to 16MB once finished testing
#define heap_size 96

extern int __bss_end__;

typedef struct hdr{
    unsigned int payload_size;
    unsigned int status;
}hdr;

typedef hdr* hdr_p; 

static char* heap_start = (char*)((&__bss_end__)); //got to the end of the bss area


void *malloc(size_t nbytes){
    void* ptr = NULL;
    nbytes = roundup(nbytes,8);
    //hdr_p start_header = (hdr_p)heap_start;
    //printf("the heap_start = %d",*((int*)heap_start));
    //init the heap
    hdr_p start_header = (hdr_p)heap_start;
    if(!start_header->payload_size || start_header->payload_size > heap_size){
        printf("got initialized\n");
        start_header->payload_size = heap_size-sizeof(hdr);
        start_header->status = 0;
    }
    //printf("the heap size = %d\n",start_header->payload_size);
    //printf("the allocated space = %d\n",start_header->status);

    //process the request
    if (nbytes <= start_header->payload_size - start_header->status ){
        ptr = ((char*)start_header) + sizeof(hdr) + start_header->status;
        start_header->status += nbytes;
    } 

    return ptr;
}

void free(void *ptr){
    
}

void *realloc(void *ptr, size_t new_size){
    return 0;
}

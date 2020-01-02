#include "malloc.h"
#include "libs/printf.h"
#include "libs/timer.h"
#include "libs/strings.h"
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

void heapdump(){
    for(int i = 0; i < heap_size; i += 4){
        printf("%d|",*((int*)(heap_start+i)));
    }
    printf("\n");
}

void *malloc(size_t nbytes){
    void* ptr = NULL;
    nbytes = roundup(nbytes,8);
    
    //init the heap
    hdr_p start_header = (hdr_p)heap_start;
    if((!start_header->payload_size && start_header->status != 1) || start_header->payload_size > heap_size){
        while(heap_start < (char*)start_header + heap_size){
            *((int*)heap_start) = 0;
            heap_start += 4;
        }

        heap_start = (char*)start_header;
        start_header->payload_size = heap_size-sizeof(hdr);
        start_header->status = 0;
        printf("the heap address:%p\n",heap_start);
        heapdump();
    }

    //process the request
    while( (char*)start_header < heap_start + heap_size){
        if (start_header->payload_size >= nbytes && !start_header->status ){
            if(((char*)start_header + sizeof(hdr) + nbytes) <= heap_start + heap_size && start_header->payload_size > 8)
                ((hdr_p)((char*)start_header + sizeof(hdr) + nbytes))->payload_size = start_header->payload_size - (nbytes+sizeof(hdr));
            //start_header = (hdr_p)((char*)start_header + start_header->payload_size);
            start_header->status = 1;
            start_header->payload_size = nbytes;
            
            ptr = (char*)start_header + sizeof(hdr);
            break;
        }else{
            start_header = (hdr_p)((char*)start_header + sizeof(hdr) + start_header->payload_size);
        }
    } 



    return ptr;
}

void free(void *ptr){
   hdr_p start_header = ((hdr_p)((char*)ptr - sizeof(hdr)));
   start_header->status = 0;
   size_t cur_free_size = start_header->payload_size;
   start_header = (hdr_p)((char*)start_header + start_header->payload_size);//go one node to the right
   while((char*)start_header < heap_start+heap_size && !start_header->status){
       cur_free_size += start_header->payload_size + sizeof(hdr);
       start_header = (hdr_p)((char*)start_header + start_header->payload_size);
       if(!start_header->payload_size){
           start_header = (hdr_p)((char*)start_header+sizeof(hdr));
           //cur_free_size -= sizeof(hdr);
       }
   }
   start_header = ((hdr_p)((char*)ptr - sizeof(hdr)));
   start_header->payload_size = cur_free_size - sizeof(hdr);
}

void *realloc(void *ptr, size_t new_size){
    new_size = roundup(new_size, 8);
    void* temp = ptr;
    size_t curr_size;
    hdr_p start_header = (hdr_p)((char*)ptr-sizeof(hdr));
    curr_size = start_header->payload_size;
    if ( start_header->payload_size < new_size ){//check if we are not shrinking
        start_header = (hdr_p)((char*)start_header + sizeof(hdr) + start_header->payload_size);

        while( (char*)start_header < heap_start+heap_size && !start_header->status && curr_size < new_size){
            curr_size += start_header->payload_size+sizeof(hdr);
            start_header = (hdr_p)((char*)start_header + sizeof(hdr) + start_header->payload_size);
        }

        if(curr_size >= new_size){//reallocate in place
            start_header = (hdr_p)((char*)ptr - sizeof(hdr));
            start_header->payload_size = new_size;
            if(((char*)start_header + sizeof(hdr) + new_size) <= heap_start + heap_size)
                 ((hdr_p)((char*)start_header + sizeof(hdr) + new_size))->payload_size = curr_size - (new_size+sizeof(hdr));
        }

        else{//couldn't reallocate in place
            ptr = malloc(new_size);
            if (ptr){
                ptr = memcpy(ptr, temp, ((hdr_p)((char*)temp-sizeof(hdr)))->payload_size);
                ((hdr_p)((char*)temp-sizeof(hdr)))->status = 0;
            }
        }
    } 
    return ptr;
}

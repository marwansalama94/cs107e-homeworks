#include "backtrace.h"
#include "libs/printf.h"

static const char* invalid_name = "???";

typedef struct{
    //the aapcs64 is a little different it doesn't save the pc nor the ip
    unsigned long int saved_fp;  // points to the frame pointer of the previous function
    unsigned long int saved_lr;  // points the the address after the function executes
    unsigned long int garbage1;
    unsigned long int garbage2;
    }saved_registers_t;

typedef saved_registers_t* saved_registers_p;



int backtrace(frame_t f[], int max_frames){
    void *cur_fp;
    int i = 0;
    __asm__("mov %0, fp" : "=r" (cur_fp));//now i have the current fp
    
    saved_registers_p t = ((saved_registers_p) (cur_fp));
    //t = ((saved_registers_p) t->saved_fp); // skip the print_backtrace frame
    while((i < max_frames) && t->saved_fp){
        f[i].name = (char*)invalid_name;
        f[i].resume_addr = t->saved_lr-4;
        f[i].resume_offset = 0; //just for now till i figure something out 
        t = ((saved_registers_p) t->saved_fp);
        i++;
    }
    return i;
}

void print_frames(frame_t f[], int n){
   int i;
   for(i = 0; i < n; i++){
       printf("#%d %x at %s+%d\n",i ,(unsigned int)f[i].resume_addr, f[i].name, f[i].resume_offset);
   }
}

void print_backtrace(void){
    //let's say we gonna print only at max 10 frames
    frame_t f[10];
    print_frames(f, backtrace(f,10));
}

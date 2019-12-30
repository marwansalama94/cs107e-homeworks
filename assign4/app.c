#include "libs/printf.h"
#include "libs/gpio.h"
#include "libs/timer.h"
#include "libs/uart.h"
#include "backtrace.h"
#include "malloc.h"

int main(void)
{
    
    gpio_init();
    timer_init();
    uart_init();
    int* a;
    int* b;
    int* c;
    int* d;
    int* e;
    int* f;
    int* g;
    a = (int*)malloc(10);
    b = (int*)malloc(8);
    c = (int*)malloc(20);
    d = (int*)malloc(4);
    e = (int*)malloc(7);
    f = (int*)malloc(40);
    g = (int*)malloc(8);
    while(1){
        printf("malloc test:\n ============\n");
        printf("a malloc(10) returns:%p\n",a);
        printf("b malloc(8) returns:%p\n",b);
        printf("c malloc(20) returns:%p\n",c);
        printf("d malloc(4) returns:%p\n",d);
        printf("e malloc(7) returns:%p\n",e);
        printf("f malloc(40) returns:%p\n",f);
        printf("g malloc(25) returns:%p\n",g);
        printf("====================\n");
        timer_delay(5);
    }
}

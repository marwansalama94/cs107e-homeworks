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
    int* h;
    a = (int*)malloc(10);
    printf("after a malloc(10):\n");
    heapdump();
    free(a);
    printf("after free(a): \n");
    heapdump();
    a = (int*)malloc(8);
    printf("after a malloc(8):\n");
    heapdump();
    a = realloc(a,20);
    printf("a realloc(a,20) returns: %p\n",a);
    heapdump();
    b = (int*)malloc(8);
    printf("after b malloc(8):\n");
    heapdump();
    c = (int*)malloc(20);
    printf("after c malloc(20):\n");
    heapdump();
    free(b);
    printf("after  free(b):\n");
    heapdump();
    free(c);
    printf("after  free(c):\n");
    heapdump();
    d = (int*)malloc(4);
    printf("after d malloc(4):\n");
    heapdump();
    e = (int*)malloc(7);
    printf("after e malloc(7):\n");
    heapdump();
    f = (int*)malloc(40);
    free(d);
    g = (int*)malloc(8);
    h = (int*)malloc(8);
    while(1){
        timer_delay(5);
    }
}

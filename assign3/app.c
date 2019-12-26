#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "printf.h"

void main(void){
    gpio_init();
    timer_init();
    uart_init();
    volatile char fck[5] = "ok";
    volatile int x = 40;
    
    while(1){
        //timer_delay(2);
        printf("Hello Laptop\n the value of x in decimal is %d,\n in hexdecimal is %x\n and it's address is %p\n",x,x,&x);
    }
}

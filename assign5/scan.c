#include "libs/gpio.h"
#include "keyboard.h"
#include "libs/uart.h"
#include "libs/printf.h"
#include "libs/timer.h"
#include "pi.h"
#include "shell.h"


void main(void) 
{
    gpio_init();
    timer_init();
    keyboard_init();
    uart_init();
    shell_init(printf);

    while (1) {
        //printf("gonna reboot\n");
        shell_run();
        
    }
}

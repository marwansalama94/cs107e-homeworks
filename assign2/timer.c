#include "timer.h"
#define TIMER_BASE 0x3F00B400

struct timer_registers{
  unsigned int load; //0
  unsigned int value; // 1
  unsigned int control; // 2
  unsigned int irq_clear_ack; //3
  unsigned int raw_irq;//4
  unsigned int masked_irq;//5
  unsigned int reload;//6
  unsigned int pre_divider;//7
  unsigned int free_running_counter;//8
};

static volatile struct timer_registers* registers;

/*
 * Initialize the timer. For assignment 2, this does nothing.
 * However, all libpi peripheral modules require an init, so it is
 * included for consistency's sake.
 */

void timer_init(void){
  registers = (struct timer_registers*)TIMER_BASE;
  registers->control = 0x00F90200 | 0x00F90000; //put value in the control register
}

/*
 * Returns the current system tick count. The tick count is set to
 * zero at boot and increments once per microsecond until the
 * processor resets.  One tick is equal to one microsecond.
 *
 * @return  system tick count
 */
unsigned int timer_get_ticks(void){
  return registers->free_running_counter;
}

/*
 * A simple busy loop that delays the program for `usec` microseconds.
 *
 * @param usec  the number of microseconds to busy loop for
 */
void timer_delay_us(unsigned int usec){
  unsigned int start = timer_get_ticks();
  while ( (timer_get_ticks() - start) < usec) { /* spin */ }
}

/*
 * Delay for `msec` milliseconds.
 *
 * @param msec  the number of milliseconds to busy loop for
 */
void timer_delay_ms(unsigned int msec){
  timer_delay_us(msec*1000);
}

/*
 * Delay for `sec` seconds.
 *
 * @param sec the number of seconds to busy loop for
 */
void timer_delay(unsigned int sec){
  timer_delay_us(sec*1000000);
}

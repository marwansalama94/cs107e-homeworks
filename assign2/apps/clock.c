#include "../gpio.h"
#include "../timer.h"

#define DBASE 10
#define SBASE 20

const static unsigned int bit_patterns[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x18};

static void pins_init(void){
    for(int i = DBASE; i < DBASE+4; i++){//set the selector pins
        gpio_set_output(i);
    }

    for(int i = SBASE; i < SBASE+7; i++){ // set the segments pins
        gpio_set_output(i);
        gpio_write(i,1);
    }
}

void represent_number(unsigned int pin,unsigned int digit){
  unsigned int bit_pattern = bit_patterns[digit];
    for(int i = 0; i < 4; i++){
        gpio_write(DBASE+i,0);
    }
    gpio_write(pin,1);
  for(int i = 0; i < 7; i++){
    gpio_write(20+i,bit_pattern&1);
    bit_pattern >>= 1;
  }
}

void write_seconds(unsigned int secs){
    timer_delay_ms(1);
    represent_number(DBASE+3, secs%10);
    timer_delay_ms(1);
    represent_number(DBASE+2, secs/10);
}

void write_minutes(unsigned int mins){
    timer_delay_ms(1);
    represent_number(DBASE+1, mins%10);
    timer_delay_ms(1);
    represent_number(DBASE, mins/10);
}

void refresh_clock(unsigned int secs,unsigned int mins){
    write_seconds(secs);
    write_minutes(mins);
}



unsigned int calculate_minutes(unsigned int ticks){
    return (ticks/1000000)/60;
}

unsigned int calculate_seconds(unsigned int ticks){
    return ((ticks/1000000))-(calculate_minutes(ticks)*60);
}

void main(void) {
    unsigned int start_offset,current_tick_count;
  gpio_init();
  timer_init();
  pins_init();
    start_offset = timer_get_ticks();
    
  while(1) {
      current_tick_count = timer_get_ticks()-start_offset;
      refresh_clock(calculate_seconds(current_tick_count), calculate_minutes(current_tick_count));
      
  }
}

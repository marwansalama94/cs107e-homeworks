#include "libs/gpio.h"
#include "keyboard.h"
#include "libs/printf.h"
#include "libs/ps2.h"
#include "libs/strings.h"
#include "libs/timer.h"

static const unsigned CLK  = GPIO_PIN23;
static const unsigned DATA = GPIO_PIN24; 

static unsigned int current_modifiers = 0;

void keyboard_init(void) 
{
    gpio_set_input(CLK);
    gpio_set_pullup(CLK); 
 
    gpio_set_input(DATA);
    gpio_set_pullup(DATA); 
}

unsigned char keyboard_read_next(void){
    key_event_t event = keyboard_read_event();
    unsigned char ch = 0;
    if (event.key.ch <= 0x7F && !event.action){
        ch = ((current_modifiers & KEYBOARD_MOD_SHIFT)) ? event.key.other_ch :event.key.ch;
    }else{
        ch = 0;
    }
    return ch;
}

key_event_t keyboard_read_event(void){
   key_event_t event;
   event.seq_len = keyboard_read_sequence(event.seq);
   event.key = ps2_keys[event.seq[event.seq_len-1]];
   switch(event.seq_len){
        case 1:
            event.action = KEYBOARD_ACTION_DOWN;
           break;
        case 2:
           if(event.seq[0] == PS2_CODE_RELEASE) event.action = KEYBOARD_ACTION_UP;
           else event.action = KEYBOARD_ACTION_DOWN;
           break;
        case 3:
           event.action = KEYBOARD_ACTION_UP;
           break;
        default:
           break;
   }
    
   event.modifiers = current_modifiers;
    
   if(event.key.ch == PS2_KEY_SHIFT)
       current_modifiers = (event.action == KEYBOARD_ACTION_DOWN ) ? current_modifiers | KEYBOARD_MOD_SHIFT : current_modifiers & ~KEYBOARD_MOD_SHIFT;   
   if(event.key.ch == PS2_KEY_ALT)
       current_modifiers = (event.action == KEYBOARD_ACTION_DOWN ) ? current_modifiers | KEYBOARD_MOD_ALT : current_modifiers & ~KEYBOARD_MOD_ALT;
   return event;
}


static void wait_for_falling_clock_edge() 
{
    while (gpio_read(CLK) == 0) {}
    while (gpio_read(CLK) == 1) {}
}

unsigned char keyboard_read_scancode(void) 
{
    //unsigned int timeout = timer_get_ticks();
    int count = 0;
    unsigned char key = 0;
    unsigned char high_bit_count = 0;
    while(count < 11){
        //if(timer_get_ticks()-timeout > 3*1000) return 0;
        wait_for_falling_clock_edge();
        //timeout = timer_get_ticks();
        if(count > 0 && count < 9){
            unsigned int bit = gpio_read(DATA);
            key += bit<<(count-1);
            if(bit) high_bit_count++;
        }else if(count == 9){
            if(((gpio_read(DATA) + high_bit_count)%2) == 0){
                count = 0;
                high_bit_count = 0;
                key = 0;
                wait_for_falling_clock_edge();
            }
        }
        count++;
        //if (count == 9){
           // wait_for_falling_clock_edge();
           // unsigned char parity = gpio_read(DATA);
           // if (!((high_bit_count % 2 == 0 && parity) || (high_bit_count % 2 !=0 && !parity))){
                //wait_for_falling_clock_edge();//skip the stop bit
                //key = 0;
            //    count = 0;
              //  high_bit_count = 0;
          //  }
        //}
    }
    return key;
}

int keyboard_read_sequence(unsigned char seq[])
{

    seq[0] = keyboard_read_scancode();
    if(seq[0] == PS2_CODE_EXTEND){
        seq[1] = keyboard_read_scancode();
        if(seq[1] == PS2_CODE_RELEASE){
            seq[2] = keyboard_read_scancode();
            return 3;
        }
    }else if (seq[0] == PS2_CODE_RELEASE){
        seq[1] = keyboard_read_scancode();
        return 2;
    }
    return 1;
}

void keyboard_use_interrupts(void){
    
}

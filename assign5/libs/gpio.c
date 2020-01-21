#include "gpio.h"
#include <stddef.h>

#define base 0x3F200000


struct gpio{
    unsigned int FSEL[6];
    unsigned int res1;
    unsigned int SET[2];
    unsigned int res2;
    unsigned int CLR[2];
    unsigned int res3;
    unsigned int LEV[2];
    unsigned int res4;
    unsigned int EDS[2];
    unsigned int res5;
    unsigned int REN[2];
    unsigned int res6;
    unsigned int FEN[2];
    unsigned int HEN[2];
    unsigned int res7;
    unsigned int LEN[2];
    unsigned int res8;
    unsigned int AREN[2];
    unsigned int res9;
    unsigned int AFEN[2];
    unsigned int res10;
    unsigned int PUD;
    unsigned int PUDCLK[2];
};

static volatile struct gpio* _gpio;


void gpio_init(void){
  _gpio = (struct gpio*)base;
}


void gpio_set_function(unsigned int pin, unsigned int function){
  unsigned int offset = 0;
  unsigned volatile int *reg;
  if (pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST && function >=0 && function <= 7){
    if ( (reg = &_gpio->FSEL[pin/10]) ){
      offset = pin % 10;
      *reg = (*reg | ( function << ( offset * 3) ));
    }
  }
}


unsigned int gpio_get_function(unsigned int pin){
  unsigned int offset = 0;
  unsigned volatile int* reg = &_gpio->FSEL[pin/10];
  if(reg){
    offset = pin % 10;
    return (*reg >> (offset * 3)) & 7;
  }
  return GPIO_INVALID_REQUEST;
}



void gpio_set_input(unsigned int pin){
  gpio_set_function(pin, GPIO_FUNC_INPUT);
}

void gpio_set_output(unsigned int pin){
  gpio_set_function(pin,GPIO_FUNC_OUTPUT);
}

void gpio_write(unsigned int pin, unsigned int val){
  unsigned volatile int* reg;
  unsigned int offset = pin % 32;
  if(val){
    reg = &_gpio->SET[pin / 32] ;
  }else{
    reg = &_gpio->CLR[pin / 32];
  }
  *reg = 1 << offset;
}

unsigned int gpio_read(unsigned int pin){
  unsigned volatile int* reg = &_gpio->LEV[pin / 32];
  unsigned offset = pin % 32;
  return (*reg >> offset) & 1;
}


static volatile unsigned int *gpio_reg_for_event(unsigned int event)
{
    volatile unsigned int *registers[] = { _gpio->REN, _gpio->FEN, _gpio->HEN, _gpio->LEN, _gpio->AREN, _gpio->AFEN};

    if (event < 0 || event >= sizeof(registers)/sizeof(*registers)) return NULL;
    return registers[event];
}

static void gpio_set_detection(unsigned int pin, unsigned int event, unsigned int enabled)
{
    volatile unsigned int *reg = gpio_reg_for_event(event);

    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST || reg == NULL) {
        return;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;
    if (enabled)
        reg[bank] |= 1 << shift;
    else
        reg[bank] &= ~(1 << shift);
    gpio_clear_event(pin);
}

bool gpio_get_event_detection(unsigned int pin, unsigned int event)
{
    volatile unsigned int *reg = gpio_reg_for_event(event);

    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST || reg == NULL) {
        return false;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;
    return (reg[bank] & (1 << shift)) != 0;
}

void gpio_disable_event_detection(unsigned int pin, unsigned int event)
{
    gpio_set_detection(pin, event, 0);
}

void gpio_enable_event_detection(unsigned int pin, unsigned int event)
{
    gpio_set_detection(pin, event, 1);
}

bool gpio_check_event(unsigned int pin)
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return false;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;
    return (_gpio->EDS[bank] & (1 << shift)) != 0;
}

void gpio_clear_event(unsigned int pin)
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;
    if (gpio_check_event(pin))
        _gpio->EDS[bank] = 1 << shift;
}

bool gpio_check_and_clear_event(unsigned int pin)
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return false;
    }

    bool event = gpio_check_event(pin);
    if (event) gpio_clear_event(pin);
    return event;
}

void gpio_disable_all_event_detection(unsigned int pin)
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return;
    }
    for (int i = GPIO_DETECT_RISING_EDGE; i <= GPIO_DETECT_ASYNC_FALLING_EDGE; i++)
        gpio_set_detection(pin, i, 0);
    gpio_clear_event(pin);
}

/*
 * From Broadcom manual about how to use the pull-up/down:
 * -------------------------------------------------------
 * The GPIO Pull-up/down Clock Registers control the actuation of internal
 * pull-downs on the respective GPIO pins. These registers must be used
 * in conjunction with the GPPUD register to effect GPIO Pull-up/down changes.
 * The following sequence of events is required:
 * 1. Write to GPPUD to set the required control signal
 *    (i.e. Pull-up or Pull-Down or neither to remove the current Pull-up/down)
 * 2. Wait 150 cycles – this provides the required set-up time for the control signal
 * 3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish
 *    to modify – NOTE only the pads which receive a clock will be modified,
 *    all others will retain their previous state.
 * 4. Wait 150 cycles – this provides the required hold time for the control signal
 * 5. Write to GPPUD to remove the control signal
 * 6. Write to GPPUDCLK0/1 to remove the clock
 */

enum { GPIO_PUD_DISABLE = 0, GPIO_PUD_PULLDOWN = 1, GPIO_PUD_PULLUP = 2 };

static void gpio_set_pud(unsigned int pin, unsigned int pud)
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;

    _gpio->PUD = pud;
    for (volatile int i = 0; i < 150; i++) ;

    _gpio->PUDCLK[bank] = 1 << shift;
    for (volatile int i = 0; i < 150; i++) ;

    _gpio->PUDCLK[bank] = 0;
}

void gpio_set_pullup(unsigned int pin)
{
    gpio_set_pud(pin, GPIO_PUD_PULLUP);
}

void gpio_set_pulldown(unsigned int pin)
{
    gpio_set_pud(pin, GPIO_PUD_PULLDOWN);
}

void gpio_set_pullnone(unsigned int pin)
{
    gpio_set_pud(pin, GPIO_PUD_DISABLE);
}

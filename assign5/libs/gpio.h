#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

/*
 * Functions for controlling Raspberry Pi GPIO.
 * You will implement this interface in assignment 2.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 *         Philip Levis <pal@cs.stanford.edu>
 *         Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Last edited Jan 2018
 */


/*
 * These enumerated values establish symbolic names for each of the GPIO pins.
 * Although you could directly refer to pins by number, using the
 * names makes it more clear to the reader what a given value represents.
 *
 * Note also the values GPIO_PIN_FIRST and GPIO_PIN_LAST that bound
 * the range of valid GPIO pin numbers.
 */
enum {
    GPIO_PIN_FIRST = 0,
    GPIO_PIN0 = 0,
    GPIO_PIN1 = 1,
    GPIO_PIN2 = 2,
    GPIO_PIN3 = 3,
    GPIO_PIN4 = 4,
    GPIO_PIN5 = 5,
    GPIO_PIN6 = 6,
    GPIO_PIN7 = 7,
    GPIO_PIN8 = 8,
    GPIO_PIN9 = 9,
    GPIO_PIN10 = 10,
    GPIO_PIN11 = 11,
    GPIO_PIN12 = 12,
    GPIO_PIN13 = 13,
    GPIO_PIN14 = 14,
    GPIO_PIN15 = 15,
    GPIO_PIN16 = 16,
    GPIO_PIN17 = 17,
    GPIO_PIN18 = 18,
    GPIO_PIN19 = 19,
    GPIO_PIN20 = 20,
    GPIO_PIN21 = 21,
    GPIO_PIN22 = 22,
    GPIO_PIN23 = 23,
    GPIO_PIN24 = 24,
    GPIO_PIN25 = 25,
    GPIO_PIN26 = 26,
    GPIO_PIN27 = 27,
    GPIO_PIN28 = 28,
    GPIO_PIN29 = 29,
    GPIO_PIN30 = 30,
    GPIO_PIN31 = 31,
    GPIO_PIN32 = 32,
    GPIO_PIN33 = 33,
    GPIO_PIN34 = 34,
    GPIO_PIN35 = 35,
    GPIO_PIN36 = 36,
    GPIO_PIN37 = 37,
    GPIO_PIN38 = 38,
    GPIO_PIN39 = 39,
    GPIO_PIN40 = 40,
    GPIO_PIN41 = 41,
    GPIO_PIN42 = 42,
    GPIO_PIN43 = 43,
    GPIO_PIN44 = 44,
    GPIO_PIN45 = 45,
    GPIO_PIN46 = 46,
    GPIO_PIN47 = 47,
    GPIO_PIN48 = 48,
    GPIO_PIN49 = 49,
    GPIO_PIN50 = 50,
    GPIO_PIN51 = 51,
    GPIO_PIN52 = 52,
    GPIO_PIN53 = 53,
    GPIO_PIN_LAST =  53
};

enum gpio_event {
    GPIO_DETECT_RISING_EDGE = 0,
    GPIO_DETECT_FALLING_EDGE,
    GPIO_DETECT_HIGH_LEVEL,
    GPIO_DETECT_LOW_LEVEL,
    GPIO_DETECT_ASYNC_RISING_EDGE,
    GPIO_DETECT_ASYNC_FALLING_EDGE,
};

// GPIO pin mappings for UART
#define GPIO_TX GPIO_PIN14
#define GPIO_RX GPIO_PIN15

/*
 * These enumerated values establish symbolic names for each of the
 * available GPIO pin functions. Each pin function corresponds to
 * a particular "mode" of operation.  For example, setting a pin's 
 * function to GPIO_FUNC_INPUT configures the pin to be used as an input.
 */
enum {
    GPIO_FUNC_INPUT   = 0,
    GPIO_FUNC_OUTPUT  = 1,
    GPIO_FUNC_ALT0    = 4,
    GPIO_FUNC_ALT1    = 5,
    GPIO_FUNC_ALT2    = 6,
    GPIO_FUNC_ALT3    = 7,
    GPIO_FUNC_ALT4    = 3,
    GPIO_FUNC_ALT5    = 2,
};

#define GPIO_INVALID_REQUEST  -1  // return value for invalid request


/*
 * Initialize the GPIO code module. For assignment 2, this does nothing.
 * However, all libpi peripheral modules require an init, so it is included
 * for consistency's sake.
 */
void gpio_init(void);

/*
 * Set a GPIO function for GPIO pin number `pin`. Settings for other pins
 * should be unchanged.
 *
 * @param pin      the GPIO pin number to configure
 * @param function the GPIO function to set for the pin
 *
 * If `pin` or `function` is invalid, does nothing.
 */
void gpio_set_function(unsigned int pin, unsigned int function);

/*
 * Get the GPIO function for GPIO pin number `pin`.
 *
 * @param pin the GPIO pin number to read the function of
 * @return    the current GPIO function of the specified pin
 *
 * If `pin` is invalid, returns GPIO_INVALID_REQUEST.
 */
unsigned int gpio_get_function(unsigned int pin);

/*
 * Convenience functions for setting a pin to GPIO_FUNC_INPUT or
 * GPIO_FUNC_OUTPUT. The implementation calls `gpio_set_function`.
 *
 * @param pin the GPIO pin number to set the function of
 */
void gpio_set_input(unsigned int pin);
void gpio_set_output(unsigned int pin);

/*
 * Set GPIO pin number `pin` to high (1) or low (0). This
 * function assumes the pin is already in output mode.
 * Settings for other pins should be unchanged.
 *
 * @param pin   the GPIO pin number to set or clear
 * @param val   1 to set pin to high, 0 to clear pin 
 *
 * If `pin` is invalid, does nothing.
 */
void gpio_write(unsigned int pin, unsigned int val);

/*
 * Get current level (1 for high, 0 for low) for GPIO pin number `pin`.
 *
 * @param pin the GPIO pin number to read the value of
 * @return    the value of the specified pin
 *
 * If `pin` is invalid, returns GPIO_INVALID_REQUEST.
 */
unsigned int gpio_read(unsigned int pin);



/*
 * Enables detection of GPIO `event` for GPIO pin number `pin`.
 *
 * If `pin` or `event` is invalid, does nothing.
 */
void gpio_enable_event_detection(unsigned int pin, unsigned int event);

/*
 * Disables detection of GPIO `event` for GPIO pin number `pin`.
 *
 * If `pin` or `event` is invalid, does nothing.
 */
void gpio_disable_event_detection(unsigned int pin, unsigned int event);

/*
 * Gets the state of detection of GPIO `event` for GPIO pin number `pin`.
 * Returns true if event detection is enabled for `event`, false if disabled.
 *
 * If `pin` or `event` is invalid, returns false.
 */
bool gpio_get_event_detection(unsigned int pin, unsigned int event);

/*
 * Disables detection of all GPIO events for GPIO pin number `pin`.
 *
 * If `pin` is invalid, does nothing.
 */
void gpio_disable_all_event_detection(unsigned int pin);

/*
 * Returns the event status for GPIO pin number `pin`.
 * The event status is true if an event has been detected for `pin`
 * and the event has not been cleared, status is false otherwise.
 *
 * If `pin` is invalid, returns false.
 */
bool gpio_check_event(unsigned int pin);

/*
 * Clears the event status for GPIO pin number `pin`. Note that if the
 * event detected is high or low level and the level is still high or low,
 * clearing the event makes no change in event status.  The event
 * status remains true as long as the high or low level is in effect.
 *
 * If `pin` is invalid, does nothing.
 */
void gpio_clear_event(unsigned int pin);

/*
 * Returns the event status for GPIO pin number `pin` and clears the
 * event status.
 *
 * If `pin` is invalid, does nothing and returns false.
 */
bool gpio_check_and_clear_event(unsigned int pin);

/*
 * The functions below control the pull state of a GPIO pin via its
 * internal resistor. Setting the pull state is useful for when you
 * have an input pin that would otherwise be floating. A pin configured
 * to pull-up defaults to 1. If configured to pull-down defaults to 0.
 * The Broadcom manual says pull state of a pin cannot be queried,
 * so you have to remember what state you have configured. The manual
 * is also unclear about when the pull state is reset (may not be on
 * restart, but is after power down?). Best to reset yourself to be
 * sure.
 */

/*
 * Set pull state of GPIO pin number `pin` to pull-up.
 *
 * If `pin` is invalid, does nothing.
 */
void gpio_set_pullup(unsigned int pin);

/*
 * Set pull state of GPIO pin number `pin` to pull-down.
 *
 * If `pin` is invalid, does nothing.
 */
void gpio_set_pulldown(unsigned int pin);

/*
 * Disables any pull state of GPIO pin number `pin` (will float).
 *
 * If `pin` is invalid, does nothing.
 */
void gpio_set_pullnone(unsigned int pin);


#endif

/*****************************************************************//**
 * \file   lab01_multithreaded.cpp
 * \brief  simple LED blink program for the Raspberry Pi Pico
 *
 * This is similar to lab01.cpp but uses the multicore API to
 * run the LED blink on core 1. This allows the main core to
 * keep running and do other things.
 * 
 * \note    This doesn't work on the wokwi simulator 
 *          (it only supports single core),
 *          but it should work on real hardware.
 *
 * \author marco
 * \date   January 2025
 *********************************************************************/


#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <cstdint>


 // Global variables for LED parameters
uint32_t g_led_pin = 25;
uint32_t g_led_delay = 500;


/**
 * @brief launches the blinking logic on the second core (non-blocking)
 * 
 * Function sets global parameters then launches the core1_entry() and
 * immediately returns to the caller.
 * 
 * @param pin_num The GPIO pin number to blink
 * @param sleep_delay The delay in milliseconds between LED state changes
 */
void blink_led(uint32_t pin_num, uint32_t sleep_delay);


/**
 * @brief Entry function for core 1, which performs the actual blinking
 * 
 * Runs indefinitely, toggling the LED state at the specified interval.
 */
void core1_entry();


/**
 * @brief Main function to initialise GPIO and control LED
 * 
 * Optionally change the global variables g_led_pin and g_led_delay
 * before calling blink_led
 * 
 * @return int Return 0 for success (never returns)
 */
int main() {

    ///
	// Can change g_led_pin and g_led_delay here
    ///
    
    blink_led(g_led_pin, g_led_delay);

    while (true) {

        // Main loop is free to do something else

    }

    return 0;
}



void blink_led(uint32_t pin_num, uint32_t sleep_delay) {
    g_led_pin = pin_num;
    g_led_delay = sleep_delay;

	// Launch second core, returns immediately
    multicore_launch_core1(core1_entry);
}

void core1_entry() {
    // Initialize the assigned pin for output
    gpio_init(g_led_pin);
    gpio_set_dir(g_led_pin, GPIO_OUT);


	// Repeatedly blink the LED (only blocks this core)
    while (true) {
        gpio_put(g_led_pin, 1);
        sleep_ms(g_led_delay);

        gpio_put(g_led_pin, 0);
        sleep_ms(g_led_delay);
    }
}
/*****************************************************************//**
 * \file   lab01.cpp
 * \brief  simple LED blink program for the Raspberry Pi Pico
 * 
 * The program uses the Pico SDK to blink the onboard LED
 * at a defined interval using the RP2040 SDK.
 * 
 * \author marco
 * \date   January 2025
 *********************************************************************/


#include "pico/stdlib.h"
#include <cstdint>

 // LED GPIO pin number
constexpr uint32_t LED_PIN = 25;

// LED blink delay in milliseconds
constexpr uint32_t LED_DELAY = 500;


/**
 * @brief Blinks an LED on the specified GPIO pin at a given interval
 * 
 * @param pin_num The GPIO pin number to blink
 * @param sleep_delay The delay in milliseconds between LED state changes
 */
void blink_led(uint32_t pin_num, uint32_t sleep_delay);


/**
 * @brief Main function to initialise GPIO and control LED
 * 
 * Function initialises the GPIO pin for the LED and then
 * continuously calls the blink_led function
 * 
 * @return int Return 0 for success (never returns)
 */
int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
		blink_led(LED_PIN, LED_DELAY);
    }
    return 0;
}

void blink_led(uint32_t pin_num, uint32_t sleep_delay) {
    gpio_put(pin_num, 1); // Turn LED on
	sleep_ms(sleep_delay); // Sleep for delay

	gpio_put(pin_num, 0); // Turn LED off
	sleep_ms(sleep_delay); // Sleep for delay
}

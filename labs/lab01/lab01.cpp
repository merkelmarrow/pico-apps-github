#include "pico/stdlib.h"
#include <cstdint>

constexpr uint32_t LED_PIN = 25;
constexpr uint32_t LED_DELAY = 500;

void blink_led(uint32_t pin_num, uint32_t sleep_delay);

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
		blink_led(LED_PIN, LED_DELAY);
    }
    return 0;
}

void blink_led(uint32_t pin_num, uint32_t sleep_delay) {
    gpio_put(pin_num, 1);
    sleep_ms(sleep_delay);

    gpio_put(pin_num, 0);
    sleep_ms(sleep_delay);
}

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

void main_asm();

void asm_gpio_init(uint pin) {
    gpio_init(pin);
}

void asm_gpio_set_dir(uint pin, bool out) {
    gpio_set_dir(pin, out);
}

bool asm_gpio_get(uint pin) {
    return gpio_get(pin);
}

void asm_gpio_put(uint pin, bool value) {
    gpio_put(pin, value);
}

void asm_gpio_set_irq(uint pin) {
    gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_FALL, true);
}


int main() {
    stdio_init_all();
    main_asm();
    return 0;
}

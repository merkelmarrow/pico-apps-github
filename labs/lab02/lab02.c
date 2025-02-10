#include <stdlib.h>
#include <math.h>
#include "pico/float.h"     // Required for using single-precision variables.
#include "pico/double.h"    // Required for using double-precision variables.

//#define WOKWI             // Uncomment if running on Wokwi RP2040 emulator.

#include <stdio.h>
#include "pico/stdlib.h"

float wallis_prod_float(int n);
double wallis_prod_double(int n);

/**
 * @brief EXAMPLE - HELLO_C
 *        Simple example to initialise the IOs and then
 *        print a "Hello World!" message to the console.
 *
 * @return int  Application return code (zero for success).
 */
int main() {

#ifndef WOKWI
    // Initialise the IO as we will be using the UART
    // Only required for hardware and not needed for Wokwi
    stdio_init_all();
#endif

    // Print a console message to inform user what's going on.
    printf("Hello World!\n");

    // Returning zero indicates everything went okay.
    return 0;
}

float wallis_prod_float (int n) {
    float product = 4.0 / 3.0;
    for (int i = 2; i <= n; i++) {
        float multiplier = ((2.0 * i) / (2.0 * i - 1)) * ((2.0 * i) / (2.0 * i + 1));
        product *= multiplier;
    }
    return product;
}

double wallis_prod_double (int n) {
    double product = 4.0 / 3.0;
    for (int i = 2; i <= n; i++) {
        double multiplier = ((2.0 * i) / (2.0 * i - 1)) * ((2.0 * i) / (2.0 * i + 1));
        product *= multiplier;
    }
    return product;
}

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "pico/float.h"
#include "pico/double.h"
#include "pico/stdlib.h"


#define ITERATIONS 100000
#define ACTUAL_PI 3.14159265359

// uncomment below if running in wokwi
// #define WOKWI

float wallis_prod_float(size_t n);
double wallis_prod_double(size_t n);

int main() {

#ifndef WOKWI
	stdio_init_all();
#endif

	// give time to prepare console
	sleep_ms(10000);

	float pi_float = wallis_prod_float(ITERATIONS);
	double pi_double = wallis_prod_double(ITERATIONS);

	// Calculate errors for float and double
	float error_float = fabsf(pi_float - ACTUAL_PI);
	double error_double = fabs(pi_double - ACTUAL_PI);
	float percentage_error_float = (error_float / ACTUAL_PI) * 100.0f;
	double percentage_error_double = (error_double / ACTUAL_PI) * 100.0;

	printf("\n\n\n\t   Precision   |   Calculated PI   |   Absolute Error   |   Percentage Error   \n");
	printf("\t-------------------------------------------------------------------------------\n");
	printf("\t     Float     |   %.11f   |   %.11f    |     %.11f%%\n", pi_float, error_float, percentage_error_float);
	printf("\t-------------------------------------------------------------------------------\n");
	printf("\t     Double    |   %.11lf   |   %.11lf    |     %.11lf%%\n\n", pi_double, error_double, percentage_error_double);


	return 0;
}

float wallis_prod_float(size_t n) {
	float product = 1.0f;
	for (size_t i = 1; i <= n; i++) {
		float term = (2.0f * i) / (2.0f * i - 1.0f);
		product *= term;
		product *= (2.0f * i) / (2.0f * i + 1.0f);
	}
	return product * 2.0f;
}

double wallis_prod_double(size_t n) {
	double product = 1.0;
	for (size_t i = 1; i <= n; i++) {
		double term = (2.0 * i) / (2.0 * i - 1.0);
		product *= term;
		product *= (2.0 * i) / (2.0 * i + 1.0);
	}
	return product * 2.0;
}

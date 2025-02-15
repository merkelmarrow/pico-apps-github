/*****************************************************************//**
 * \file   lab02.c
 * \brief  approximation of pi using wallis product, comparing precision
 *		   of float and double
 * 
 * \author marco
 * \date   February 2025
 *********************************************************************/

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


/**
 * @brief computes pi approximation using wallis product formula with single precision floats
 * 
 * pi / 2 = product from i = 1 to n of [(2i / (2i - 1)) * (2i / (2i + 1))]
 * multiply the final product by 2 to get pi approximation
 * 
 * @param n Number of iterations
 */
float wallis_prod_float(size_t n);


/**
 * @brief computes pi approximation using wallis product formula with double precision floats
 * 
 * calculation is identical to wallis_prod_float except with doubles instead of floats
 * 
 * @param n Number of iterations
 */
double wallis_prod_double(size_t n);


/**
 * @brief main computes pi approximations and prints their values and the associated errors
 */
int main() {
	// needed for hardware (initialises USB input/output)
	// not needed for Wokwi simulator
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

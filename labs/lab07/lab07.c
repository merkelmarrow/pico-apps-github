#include <stdio.h>

// Must declare the main assembly entry point before use.
void main_asm();

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
 * @brief LAB #07 - TEMPLATE
 *        Main entry point for the code - calls the main assembly
 *        function where the body of the code is implemented.
 *
 * @return int      Returns exit-status zero on completion.
 */
int main() {

  // Jump into the main assembly code subroutine.
  main_asm();

  // Returning zero indicates everything went okay.
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

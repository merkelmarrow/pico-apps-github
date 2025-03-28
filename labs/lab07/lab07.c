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
 * @brief entry-point for core 1
 * 
 * function pointer is passed in via the FIFO with one incoming int32_t used as a parameter
 * The function will provide an int32_t return value by pushing it back on the FIFO
 * which indicates the result is ready
 */

 void core1_entry() {
  while (1) {
      // get the function pointer from the FIFO
      void (*func)() = (void(*)()) multicore_fifo_pop_blocking();
      int32_t p = multicore_fifo_pop_blocking();
      
      // take snapshot of timer
      uint64_t start_time = time_us_64();
      
      // call the function (single or double precision)
      if (func == (void*)calculate_pi_single) {
          float result = calculate_pi_single((int)p);
      } else {
          double result = calculate_pi_double((int)p);
      }
      
      // take snapshot of timer and calculate execution time
      uint64_t end_time = time_us_64();
      uint64_t execution_time = end_time - start_time;
      
      // return the execution time
      multicore_fifo_push_blocking(execution_time);
  }
}

bool get_xip_cache_en() {
  // the cache enable bit is bit 0 of the XIP_CTRL register
  return (*(volatile uint32_t*)(XIP_CTRL_BASE) & 0x01) != 0;
}


/**
 * @brief LAB #07 - TEMPLATE
 *        
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

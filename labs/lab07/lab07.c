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
      if (func == (void*)wallis_prod_float) {
          float result = wallis_prod_float((int)p);
      } else {
          double result = wallis_prod_double((int)p);
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
 * @brief set the enable status of the XIP cache
 * 
 * @param cache_en true to enable cache, false to disable
 * @return bool the previous cache enable state
 */
bool set_xip_cache_en(bool cache_en) {
  // get the current state
  bool prev_state = get_xip_cache_en();
  
  // set the new state
  uint32_t reg = *(volatile uint32_t*)(XIP_CTRL_BASE);
  if (cache_en) {
      reg |= 0x01; // set bit 0
  } else {
      reg &= ~0x01; // clear bit 0
  }
  *(volatile uint32_t*)(XIP_CTRL_BASE) = reg;
  
  return prev_state;
}

/**
 * @brief runs and prints wallis time test results
 * NB: single core only
 */
void wallis_time_test_single_core();

int main() {
  const int ITER_MAX = 100000;
  stdio_init_all();

  sleep_ms(5000); // wait a few seconds to connect to the serial output

  multicore_launch_core1(core1_entry);

  // scenario 1: single core with cache enabled
  printf("--Scenario 1: single core, cache enabled--\n");
  set_xip_cache_en(true);

  printf("Cache status: %s\n", get_xip_cache_en() ? "Enabled" : "Disabled");
  wallis_time_test_single_core();


  // scenario 2: single core with cache disabled
  printf("--Scenario 2: single core, cache disabled--\n");
  set_xip_cache_en(false);

  printf("Cache status: %s\n", get_xip_cache_en() ? "Enabled" : "Disabled");
  wallis_time_test_single_core();

  // scenario 3: double core with cache enabled
  printf("--Scenario 3: dual core, cache enabled--\n");
  set_xip_cache_en(true);

  // store the timing results
  uint64_t single_time, double_time, total_time;
  double pi_double;

  start_time = time_us_64();

  printf("Cache status: %s\n", get_xip_cache_en() ? "Enabled" : "Disabled");
  multicore_fifo_push_blocking((uintptr_t)&wallis_prod_float);
  multicore_fifo_push_blocking(ITER_MAX);

  uint64_t core0_start = time_us_64();
  pi_double = wallis_prod_double(ITER_MAX);
  uint64_t core0_end = time_us_64();
  double_time = core0_end - core0_start;
  
  // wait for core 1 to finish and get its timing
  single_time = multicore_fifo_pop_blocking();

  end_time = time_us_64();
  total_time = end_time - start_time;

  printf("Single precision pi time (us) = %f\n", single_time);
  printf("Double precision pi time (us) = %f\n", double_time);
  printf("Total time = %f\n", total_time);

  // scenario 4: double core with cache disabled
  printf("--Scenario 4: dual core, cache disabled--\n");
  set_xip_cache_en(false);

  start_time = time_us_64();

  printf("Cache status: %s\n", get_xip_cache_en() ? "Enabled" : "Disabled");
  multicore_fifo_push_blocking((uintptr_t)&wallis_prod_float);
  multicore_fifo_push_blocking(ITER_MAX);

  uint64_t core0_start = time_us_64();
  pi_double = wallis_prod_double(ITER_MAX);
  uint64_t core0_end = time_us_64();
  double_time = core0_end - core0_start;

  // wait for core 1 to finish and get its timing
  single_time = multicore_fifo_pop_blocking();

  end_time = time_us_64();
  total_time = end_time - start_time;

  printf("Single precision pi time (us) = %f\n", single_time);
  printf("Double precision pi time (us) = %f\n", double_time);
  printf("Total time = %f\n", total_time);

  return 0;
}

void wallis_time_test_single_core() {
  // run the single-precision wallis product
  uint64_t start_time = time_us_64();
  float pi_single = wallis_prod_float(ITER_MAX);
  uint64_t end_time = time_us_64();
  uint64_t single_time = end_time - start_time;
  printf("Single precision pi time (us) = %f\n", single_time);

  // run the double-precision wallis product
  start_time = time_us_64();
  pi_double = wallis_prod_double(ITER_MAX);
  end_time = time_us_64();
  double_time = end_time - start_time;
  printf("Double precision pi time (us) = %f\n", double_time);
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

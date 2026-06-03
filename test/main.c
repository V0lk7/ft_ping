#include "greatest.h"

// Define suite here
// SUITE(name_suite)
SUITE(icmp_suite);
SUITE(ip_suite);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN(); /*	command line initialization. */

  //   RUN_SUITE(test_tmp);
  RUN_SUITE(icmp_suite);
  RUN_SUITE(ip_suite);
  GREATEST_MAIN_END(); /* display result */
}
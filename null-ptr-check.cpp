#include "null-ptr-check.h"
#include "syntax-utils.h"

#include <vector>

using namespace std;

unsigned int
check_for_null_ptrs(function* fun) {
  vector<gimple> null_tests;
  find_all_null_tests_in_function(fun, null_tests);
  for (vector<gimple>::iterator stit = null_tests.begin(); stit != null_tests.end(); ++stit) {
    gimple st = *stit;
    warning_at(gimple_location(st), 0, "Null test");
  }
  return 0;
}

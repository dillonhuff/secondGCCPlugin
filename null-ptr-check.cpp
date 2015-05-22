#include "null-ptr-check.h"
#include "syntax-utils.h"

#include <vector>
#include <utility>

using namespace std;

unsigned int
check_for_null_ptrs(function* fun) {
  vector<pair<gimple, tree> > null_tests;
  find_all_null_tests_in_function(fun, null_tests);
  report_possible_null_ptrs(fun, null_tests);
  return 0;
}

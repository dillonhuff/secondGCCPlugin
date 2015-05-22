#include "div-zero-check.h"
#include "div-zero-checker.h"
#include "cfg-div-zero-checker.h"
#include "flagrant-div-zero-checker.h"
#include "syntax-utils.h"

#include <vector>

using namespace std;

int
check_all_divides(vector<gimple> stmts,
		  vector<div_zero_checker*> checkers) {
  for (vector<div_zero_checker*>::iterator it = checkers.begin(); it != checkers.end(); ++it) {
    for (vector<gimple>::iterator stit = stmts.begin(); stit != stmts.end(); ++stit) {
      gimple stmt = *stit;
      div_zero_checker* checker = *it;
      if (checker->check(stmt)) {
	warning_at(gimple_location(stmt), 0, "Potential division by 0");
      }
    }
  }
  return 0;
}

int
check_fun_for_div_by_zero_with_checkers(function* fun,
					vector<div_zero_checker*> checkers) {
  vector<gimple> stmts_with_divides;
  find_all_divides_in_function(fun, stmts_with_divides);
  check_all_divides(stmts_with_divides, checkers);
  return 0;
}

unsigned int
check_for_div_by_zero(function* fun) {
  vector<div_zero_checker*> checkers;
  flagrant_div_zero_checker flagrant_checker;
  checkers.push_back(&flagrant_checker);
  cfg_div_zero_checker cfg_checker;
  checkers.push_back(&cfg_checker);
  check_fun_for_div_by_zero_with_checkers(fun, checkers);
  return 0;
}

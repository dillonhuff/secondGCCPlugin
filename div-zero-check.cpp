#include "div-zero-check.h"
#include "div-zero-checker.h"
#include "cfg-div-zero-checker.h"
#include "flagrant-div-zero-checker.h"

#include "gimple-pretty-print.h"

#include <vector>

using namespace std;

unsigned int
rhs_is_a_divide(gimple asg) {
  tree_code operation_type = gimple_expr_code(asg);
  switch(operation_type) {
  case(TRUNC_DIV_EXPR):
  case(FLOOR_DIV_EXPR):
  case(CEIL_DIV_EXPR):
  case(ROUND_DIV_EXPR):
  case(RDIV_EXPR):
    return 1;
  default:
    return 0;
  }
  return 0;
}

unsigned int
denominator_of_divide_is_zero(gimple asg) {
  tree denominator = gimple_assign_rhs2(asg);
  return integer_zerop(denominator);
}

unsigned int
check_assign_for_div_by_zero(gimple asg) {
  if (rhs_is_a_divide(asg) &&
      denominator_of_divide_is_zero(asg)) {
    warning_at(gimple_location(asg), 0, "Flagrant divide by 0");
  }
  return 0;
}

unsigned int
check_flagrant_div_by_zero(gimple stmt) {
  if (gimple_code(stmt) == GIMPLE_ASSIGN) {
    check_assign_for_div_by_zero(stmt);
  }
  return 0;
}

unsigned int
rhs_is_unmodified_function_parameter(gimple asg) {
  tree denominator = gimple_assign_rhs2(asg);
  if (TREE_CODE(denominator) == SSA_NAME) {
    gimple def_statement = SSA_NAME_DEF_STMT(denominator);
    if (gimple_code(def_statement) == GIMPLE_NOP) {
      warning_at(gimple_location(asg), 0, "Divide by 0 function parameter");
    }
  }
  return 0;
}

unsigned int
check_div_by_unmodified_function_argument(gimple stmt) {
  if (gimple_code(stmt) == GIMPLE_ASSIGN) {
    gimple asg = stmt;
    return rhs_is_a_divide(asg) && rhs_is_unmodified_function_parameter(asg);
  }
  return 0;
}

int
stmt_contains_divide(gimple stmt) {
  if (gimple_code(stmt) == GIMPLE_ASSIGN) {
    return rhs_is_a_divide(stmt);
  }
  return 0;
}

int
find_all_divides_in_function(function* fun, vector<gimple>& divide_stmts) {
  basic_block bb;
  gimple stmt;
  gimple_stmt_iterator gsi;

  FOR_EACH_BB_FN(bb, fun) {
    for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
      stmt = gsi_stmt(gsi);
      if (stmt_contains_divide(stmt)) {
	divide_stmts.push_back(stmt);
      }
    }
  }
}

int
check_all_divides(vector<gimple> stmts,
		  vector<div_zero_checker*> checkers) {
  for (vector<div_zero_checker*>::iterator it = checkers.begin(); it != checkers.end(); ++it) {
    for (vector<gimple>::iterator stit = stmts.begin(); stit != stmts.end(); ++stit) {
      gimple stmt = *stit;
      div_zero_checker* checker = *it;
      if (checker->check(stmt)) {
	warning_at(gimple_location(stmt), 0, "Potential divide by 0");
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

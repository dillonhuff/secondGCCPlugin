#include "syntax-utils.h"

#include "tree-flow.h"

using namespace std;

int
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

int
denominator_of_divide_is_zero(gimple asg) {
  tree denominator = gimple_assign_rhs2(asg);
  return integer_zerop(denominator);
}

int
check_assign_for_div_by_zero(gimple asg) {
  if (rhs_is_a_divide(asg) &&
      denominator_of_divide_is_zero(asg)) {
    return 1;
  }
  return 0;
}

int
check_flagrant_div_by_zero(gimple stmt) {
  if (gimple_code(stmt) == GIMPLE_ASSIGN) {
    return check_assign_for_div_by_zero(stmt);
  }
  return 0;
}

int
rhs_is_unmodified_function_parameter(gimple asg) {
  tree denominator = gimple_assign_rhs2(asg);
  if (TREE_CODE(denominator) == SSA_NAME) {
    gimple def_statement = SSA_NAME_DEF_STMT(denominator);
    if (gimple_code(def_statement) == GIMPLE_NOP) {
      return 1; //warning_at(gimple_location(asg), 0, "Divide by 0 function parameter");
    }
  }
  return 0;
}

int denominator_is_used_exactly_once(gimple stmt) {
  tree denominator = gimple_assign_rhs2(stmt);
  if (TREE_CODE(denominator) == SSA_NAME) {
    return has_single_use(denominator);
  }
  return 0;
}

int
check_div_by_unmodified_function_argument_used_exactly_once(gimple stmt) {
  if (gimple_code(stmt) == GIMPLE_ASSIGN) {
    gimple asg = stmt;
    return rhs_is_a_divide(asg) &&
      rhs_is_unmodified_function_parameter(asg) &&
      denominator_is_used_exactly_once(asg);
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
is_ptr(tree t) {
  tree type = TREE_TYPE(t);
  return TREE_CODE(type) == POINTER_TYPE;
}

int
is_lhs_ptr_null_test(gimple cond) {
  tree lhs = gimple_cond_lhs(cond);
  tree rhs = gimple_cond_rhs(cond);
  return is_ptr(lhs) && integer_zerop(rhs);
}

int
is_rhs_ptr_null_test(gimple cond) {
  tree lhs = gimple_cond_lhs(cond);
  tree rhs = gimple_cond_rhs(cond);
  return is_ptr(rhs) && integer_zerop(lhs);
}

int
is_ptr_null_test(gimple cond) {
  return is_lhs_ptr_null_test(cond) ||
    is_rhs_ptr_null_test(cond);
}

int
is_cond_null_test(gimple cond) {
  switch(gimple_cond_code(cond)) {
  case(NE_EXPR):
  case(EQ_EXPR):
    return is_ptr_null_test(cond);
  default:
    return 0;
  }
  return 0;
}

int
is_null_test(gimple stmt) {
  if (gimple_code(stmt) == GIMPLE_COND) {
    return is_cond_null_test(stmt);
  }
  return 0;
}

int find_all_null_tests_in_function(function* fun, vector<gimple>& null_tests) {
  basic_block bb;
  gimple stmt;
  gimple_stmt_iterator gsi;

  FOR_EACH_BB_FN(bb, fun) {
    for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
      stmt = gsi_stmt(gsi);
      if (is_null_test(stmt)) {
	null_tests.push_back(stmt);
      }
    }
  }
}

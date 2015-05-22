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
      return 1;
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

tree
is_lhs_ptr_null_test(gimple cond) {
  tree lhs = gimple_cond_lhs(cond);
  tree rhs = gimple_cond_rhs(cond);
  if (is_ptr(lhs) && integer_zerop(rhs)) {
    return lhs;
  }
  return NULL_TREE;
}

tree
is_rhs_ptr_null_test(gimple cond) {
  tree lhs = gimple_cond_lhs(cond);
  tree rhs = gimple_cond_rhs(cond);
  if (is_ptr(rhs) && integer_zerop(lhs)) {
    return rhs;
  }
  return NULL_TREE;
}

tree
is_ptr_null_test(gimple cond) {
  tree lhs_res = is_lhs_ptr_null_test(cond);
  if (lhs_res != NULL_TREE) {
    return lhs_res;
  }
  return is_rhs_ptr_null_test(cond);
}

tree
is_cond_null_test(gimple cond) {
  switch(gimple_cond_code(cond)) {
  case(NE_EXPR):
  case(EQ_EXPR):
    return is_ptr_null_test(cond);
  default:
    return NULL_TREE;
  }
  return NULL_TREE;
}

tree
is_null_test(gimple stmt) {
  if (gimple_code(stmt) == GIMPLE_COND) {
    return is_cond_null_test(stmt);
  }
  return NULL_TREE;
}

int
find_all_null_tests_in_function(function* fun, vector<pair<gimple, tree> >& null_tests) {
  basic_block bb;
  gimple stmt;
  gimple_stmt_iterator gsi;

  FOR_EACH_BB_FN(bb, fun) {
    for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
      stmt = gsi_stmt(gsi);
      tree possibly_null_var = is_null_test(stmt);
      if (possibly_null_var != NULL_TREE) {
	pair<gimple, tree> p(stmt, possibly_null_var);
	null_tests.push_back(p);
      }
    }
  }
}

int
does_ptr_arithmetic_on_var(gimple stmt, tree var) {
  if (gimple_code(stmt) == GIMPLE_ASSIGN) {
    if (gimple_expr_code(stmt) == POINTER_PLUS_EXPR) {
      return 1;
    }
  }
  return 0;
}

int
report_possible_null_ptr(function* fun, gimple var_stmt, tree var) {
  basic_block bb;
  gimple stmt;
  gimple_stmt_iterator gsi;

  FOR_EACH_BB_FN(bb, fun) {
    for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
      stmt = gsi_stmt(gsi);
      printf("testing possible null\n");
      if (does_ptr_arithmetic_on_var(stmt, var)) {
	warning_at(gimple_location(stmt), 0, "pointer arithmetic on possibly null variable %s", IDENTIFIER_POINTER(var));
      }
    }
  }  
}

int
report_possible_null_ptrs(function* fun, vector<pair<gimple, tree> >& null_tests) {
  for (vector<pair<gimple, tree> >::iterator stit = null_tests.begin();
       stit != null_tests.end();
       ++stit) {
    gimple st = (*stit).first;
    tree possibly_null_var = (*stit).second;
    report_possible_null_ptr(fun, st, possibly_null_var);
  }  
  return 0;
}

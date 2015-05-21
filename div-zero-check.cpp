#include "div-zero-check.h"

unsigned int
rhs_is_a_divide(gimple asg)
{
  tree_code operation_type = gimple_expr_code(asg);
  switch(operation_type) {
  case(TRUNC_DIV_EXPR):
  case(FLOOR_DIV_EXPR):
  case(CEIL_DIV_EXPR):
  case(ROUND_DIV_EXPR):
    return 1;
  default:
    return 0;
  }
  return 0;
}

unsigned int
denominator_of_divide_is_zero(gimple asg)
{
  tree denominator = gimple_assign_rhs2(asg);
  return integer_zerop(denominator);
}

unsigned int
check_assign_for_div_by_zero(gimple asg)
{
  if (rhs_is_a_divide(asg) &&
      denominator_of_divide_is_zero(asg)) {
    warning_at(gimple_location(asg), 0, "Flagrant divide by 0");
  }
  return 0;
}

unsigned int
check_flagrant_div_by_zero(gimple stmt)
{
  if (gimple_code(stmt) == GIMPLE_ASSIGN) {
    check_assign_for_div_by_zero(stmt);
  }
  return 0;
}

unsigned int check_for_div_by_zero(function* fun) {
  basic_block bb;
  gimple stmt;
  gimple_stmt_iterator gsi;

  FOR_EACH_BB_FN(bb, fun) {
    for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
      stmt = gsi_stmt(gsi);
      check_flagrant_div_by_zero(stmt);
    }
  }
  return 0;
}

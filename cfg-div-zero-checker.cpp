#include "cfg-div-zero-checker.h"
#include "syntax-utils.h"

int
cfg_div_zero_checker::check(gimple stmt) {
  return check_div_by_unmodified_function_argument(stmt);
}

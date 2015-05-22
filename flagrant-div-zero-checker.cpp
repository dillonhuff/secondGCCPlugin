#include "flagrant-div-zero-checker.h"

#include "syntax-utils.h"

int
flagrant_div_zero_checker::check(gimple stmt) {
  return check_flagrant_div_by_zero(stmt);
}

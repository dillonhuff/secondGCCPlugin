#ifndef FLAGRANT_DIV_ZERO_CHECKER_H_
#define FLAGRANT_DIV_ZERO_CHECKER_H_

#include "div-zero-checker.h"

class flagrant_div_zero_checker : public div_zero_checker {
 public:
  virtual int check(gimple stmt);
};

#endif

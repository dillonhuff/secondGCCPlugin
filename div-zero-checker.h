#ifndef DIV_ZERO_CHECKER_H_
#define DIV_ZERO_CHECKER_H_

#include "gcc-plugin.h"

class div_zero_checker {
 public:
  virtual int check(gimple stmt) { return 0; }
};

#endif

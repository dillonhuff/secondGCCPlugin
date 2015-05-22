#ifndef CFG_DIV_ZERO_CHECKER_H_
#define CFG_DIV_ZERO_CHECKER_H_

#include "div-zero-checker.h"

class cfg_div_zero_checker : public div_zero_checker {
 public:
  virtual int check(gimple stmt);
};

#endif

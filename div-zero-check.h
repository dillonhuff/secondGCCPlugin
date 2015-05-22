#ifndef DIV_ZERO_CHECK_H_
#define DIV_ZERO_CHECK_H_

#include "gcc-plugin.h"
#include "gimple.h"
#include "tree-pass.h"

unsigned int check_for_div_by_zero(function* fun);

#endif

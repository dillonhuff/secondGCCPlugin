#ifndef NULL_PTR_CHECK_H_
#define NULL_PTR_CHECK_H_

#include "gcc-plugin.h"
#include "gimple.h"

unsigned int check_for_null_ptrs(function* fun);

#endif

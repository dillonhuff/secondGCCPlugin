#ifndef SYNTAX_UTILS_H_
#define SYNTAX_UTILS_H_

#include "gcc-plugin.h"
#include "function.h"
#include "gimple.h"
#include "diagnostic-core.h"


#include <vector>

using namespace std;

int rhs_is_a_divide(gimple asg);

int denominator_of_divide_is_zero(gimple asg);

int check_assign_for_div_by_zero(gimple asg);

int check_flagrant_div_by_zero(gimple stmt);

int rhs_is_unmodified_function_parameter(gimple asg);

int check_div_by_unmodified_function_argument_used_exactly_once(gimple stmt);

int stmt_contains_divide(gimple stmt);

int find_all_divides_in_function(function* fun, vector<gimple>& divide_stmts);

int find_all_null_tests_in_function(function* fun, vector<gimple>& null_tests);

#endif

#include "gcc-plugin.h"
#include "tree-pass.h"
#include "function.h"

#include "div-zero-check.h"
#include "null-ptr-check.h"

#include <iostream>

using namespace std;

int plugin_is_GPL_compatible;

extern "C"
unsigned int
check_for_div_by_zero_execute()
{
  check_for_div_by_zero(cfun);
  return 0;
}

extern "C"
bool
check_for_div_by_zero_gate()
{
  return true;
}

static struct opt_pass check_for_div_by_zero_pass = {
  GIMPLE_PASS,
  "checkfordivbyzeropass",
  0,
  check_for_div_by_zero_gate,
  check_for_div_by_zero_execute,
};

extern "C"
unsigned int
check_for_null_ptr_execute()
{
  check_for_null_ptrs(cfun);
  return 0;
}

extern "C"
bool
check_for_null_ptr_gate()
{
  return true;
}

static struct opt_pass check_for_null_ptr_pass = {
  GIMPLE_PASS,
  "checkfornullptrpass",
  0,
  check_for_null_ptr_gate,
  check_for_null_ptr_execute,
};

extern "C" int
plugin_init (plugin_name_args* plugin_info,
             plugin_gcc_version* ver)
{
  struct register_pass_info pass_info;
  pass_info.pass = &check_for_null_ptr_pass;
  const char* ref_pass_name = "ssa";
  pass_info.reference_pass_name = ref_pass_name;
  pass_info.ref_pass_instance_number = 1;
  pass_info.pos_op = PASS_POS_INSERT_AFTER;

  register_callback(plugin_info->base_name,
		    PLUGIN_PASS_MANAGER_SETUP,
		    NULL,
		    &pass_info);

  return 0;
}

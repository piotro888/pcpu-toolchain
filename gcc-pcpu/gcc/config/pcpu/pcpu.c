#define IN_TARGET_CODE 1
//#include "pcpu.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "regs.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "output.h"
#include "calls.h"
#include "builtins.h"

/* This file should be included last.  */
#include "target-def.h"

// static void pcpu_print_operand(FILE *file, rtx x, int code){}
// static void pcpu_print_operand_address(FILE* file, machine_mode, rtx x){}
// static bool pcpu_legitimate_address_p(machine_mode mode ATTRIBUTE_UNUSED,rtx x ATTRIBUTE_UNUSED,bool strict ATTRIBUTE_UNUSED){return true;}
// static rtx pcpu_function_value(const_tree valtype,const_tree fntype_or_decl,bool outgoing){return gen_rtx_REG(TYPE_MODE(valtype), PCPU_R0);}
void pcpu_expand_prologue(){}
void pcpu_expand_epilogue(){}

// struct GTY(()) machine_function
// {
// 	// Number of bytes saved on the stack for callee saved registers
// 	int callee_save_reg_size;

// 	// Number of bytes saved on the stack for local variables
// 	int local_var_size;

// 	// The sum of 2 sizes: local vars and padding byte for sasing the 
// 	// registers. Used in expand_prologue() and expand_epilogue()
// 	int size_for_adjusting_sp;
// };

// static struct machine_function* pcpu_init_machine_status (void) {
//   return ggc_cleared_alloc<machine_function>();
// }

// static void pcpu_option_override (void) {
//   /* Set the per-function-data initializer.  */
//   init_machine_status = &pcpu_init_machine_status;
// }

// #undef TARGET_LEGITIMATE_ADDRESS_P
// #define TARGET_LEGITIMATE_ADDRESS_P pcpu_legitimate_address_p
// #undef TARGET_FUNCTION_VALUE
// #define TARGET_FUNCTION_VALUE pcpu_function_value

// #undef TARGET_OPTION_OVERRIDE
// #define TARGET_OPTION_OVERRIDE pcpu_option_override
// #undef TARGET_PRINT_OPERAND
// #define TARGET_PRINT_OPERAND pcpu_print_operand
// #undef TARGET_PRINT_OPERAND_ADDRESS
// #define TARGET_PRINT_OPERAND_ADDRESS pcpu_print_operand_address
 struct gcc_target targetm = TARGET_INITIALIZER;

//#include "gt-pcpu.h"
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


/* The PRINT_OPERAND worker */
static void pcpu_print_operand(FILE *file, rtx x, int code)
{
	rtx operand = x;

	switch(code)
	{
		case 0:
			break;
		default:
			//LOSE_AND_RETURN("invalid operand modifier lettes", x);
			break;
	}

	switch(GET_CODE(operand))
	{
		case REG:
			fprintf(file, "%s", reg_names[REGNO(operand)]);
			break;
		case MEM:
			output_address(GET_MODE(XEXP(operand, 0)), XEXP(operand, 0));
			break;
		default:
			if(CONSTANT_P(operand))
			{
				output_addr_const(file, operand);
				break;
			}
			//LOSE_AND_RETURN("unexpected operand", x);
			break;
	}

	return;
}
static void
pcpu_print_operand_address (FILE *file, machine_mode, rtx x)
{
  switch (GET_CODE (x))
    {
    case REG:
      fprintf (file, "(%s)", reg_names[REGNO (x)]);
      break;
      
    case PLUS:
      switch (GET_CODE (XEXP (x, 1)))
	{
	case CONST_INT:
	  fprintf (file, "%ld(%s)", 
		   INTVAL(XEXP (x, 1)), reg_names[REGNO (XEXP (x, 0))]);
	  break;
	case SYMBOL_REF:
	  output_addr_const (file, XEXP (x, 1));
	  fprintf (file, "(%s)", reg_names[REGNO (XEXP (x, 0))]);
	  break;
	case CONST:
	  {
	    rtx plus = XEXP (XEXP (x, 1), 0);
	    if (GET_CODE (XEXP (plus, 0)) == SYMBOL_REF 
		&& CONST_INT_P (XEXP (plus, 1)))
	      {
		output_addr_const(file, XEXP (plus, 0));
		fprintf (file,"+%ld(%s)", INTVAL (XEXP (plus, 1)),
			 reg_names[REGNO (XEXP (x, 0))]);
	      }
	    else
	      abort();
	  }
	  break;
	default:
	  abort();
	}
      break;

    default:
      output_addr_const (file, x);
      break;
    }
}
//static void pcpu_print_operand_address(FILE* file, machine_mode, rtx x){}
static bool pcpu_legitimate_address_p(machine_mode mode ATTRIBUTE_UNUSED, rtx x, bool strict ATTRIBUTE_UNUSED){
	// check if address is valid
	switch(GET_CODE(x)){
		case REG:
		case SUBREG:
			return true; // if pseudoregs def check if not pseudoreg
		case PLUS:
			return REG_P(XEXP(x, 0)) && CONST_INT_P(XEXP(x, 1));
		case CONST_INT:
			return true; 
		default:
			return false;
	}
}

static rtx pcpu_function_value(const_tree valtype, const_tree fntype_or_decl, bool outgoing) {
	//return value of function always in reg 0
	return gen_rtx_REG(TYPE_MODE(valtype), PCPU_R0);
}

static rtx pcpu_function_arg (cumulative_args_t cum_v, machine_mode mode, const_tree type ATTRIBUTE_UNUSED, bool named ATTRIBUTE_UNUSED) {
  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
  // return next reg to be used for function arg
  if (*cum < 6) //idk check later
    return gen_rtx_REG (mode, *cum);
  else 
    return NULL_RTX;
}

static void pcpu_function_arg_advance (cumulative_args_t cum_v, machine_mode mode, const_tree type, bool named ATTRIBUTE_UNUSED) {
  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
	// reserve size+1 next registers. if pointer is over avalible register use stack
  *cum = (*cum < PCPU_R6 //change to r5??
	  ? *cum + ((1 + GET_MODE_SIZE (mode)) / 2)
	  : *cum);
}

static bool pcpu_function_value_regno_p (const unsigned int regno) {
  //return value in reg 0
  return (regno == PCPU_R0);
}

struct GTY(()) machine_function
{
	// Number of bytes saved on the stack for callee saved registers
	int callee_save_reg_size;

	// Number of bytes saved on the stack for local variables
	int local_var_size;

	// The sum of 2 sizes: local vars and padding byte for sasing the 
	// registers. Used in expand_prologue() and expand_epilogue()
	int size_for_adjusting_sp;
};

void pcpu_expand_prologue(){

}

void pcpu_expand_epilogue(){
	emit_jump_insn(gen_returner());
}

static struct machine_function* pcpu_init_machine_status (void) {
  return ggc_cleared_alloc<machine_function>();
}

static void pcpu_option_override (void) {
  /* Set the per-function-data initializer.  */
  init_machine_status = &pcpu_init_machine_status;
}


#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P pcpu_legitimate_address_p

#undef TARGET_FUNCTION_VALUE
#define TARGET_FUNCTION_VALUE pcpu_function_value

#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG pcpu_function_arg
#undef TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE pcpu_function_arg_advance

#undef TARGET_FUNCTION_VALUE_REGNO_P
#define TARGET_FUNCTION_VALUE_REGNO_P pcpu_function_value_regno_p

#undef TARGET_OPTION_OVERRIDE
#define TARGET_OPTION_OVERRIDE pcpu_option_override
#undef TARGET_PRINT_OPERAND
#define TARGET_PRINT_OPERAND pcpu_print_operand
#undef TARGET_PRINT_OPERAND_ADDRESS
#define TARGET_PRINT_OPERAND_ADDRESS pcpu_print_operand_address
 struct gcc_target targetm = TARGET_INITIALIZER;

#include "gt-pcpu.h"
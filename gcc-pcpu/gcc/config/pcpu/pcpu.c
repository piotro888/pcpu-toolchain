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
#include "df.h"
#include "calls.h"

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
	  fprintf (file, "(%s), %ld", reg_names[REGNO (XEXP (x, 0))], 
		   INTVAL(XEXP (x, 1)));
	  break;
	case SYMBOL_REF:
	  fprintf (file, "(%s), ", reg_names[REGNO (XEXP (x, 0))]);
	  output_addr_const (file, XEXP (x, 1));
	  break;
	case CONST:
	  {
	    rtx plus = XEXP (XEXP (x, 1), 0);
	    if (GET_CODE (XEXP (plus, 0)) == SYMBOL_REF 
		&& CONST_INT_P (XEXP (plus, 1)))
	      {

		fprintf (file,"(%s), %ld+",
			 reg_names[REGNO (XEXP (x, 0))] , INTVAL (XEXP (plus, 1)));
			 		output_addr_const(file, XEXP (plus, 0));
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
			return (REG_P(XEXP(x, 0)) && CONST_INT_P(XEXP(x, 1)));
		case SYMBOL_REF:
		case LABEL_REF:
		case CONST:
			return true;
		default:
			return false;
	}
}

static rtx pcpu_function_value(const_tree valtype, const_tree fntype_or_decl ATTRIBUTE_UNUSED, bool outgoing ATTRIBUTE_UNUSED) {
	//return value of function always in reg 0
	return gen_rtx_REG(TYPE_MODE(valtype), PCPU_R0);
}

static rtx pcpu_libcall_value (machine_mode mode, const_rtx fun ATTRIBUTE_UNUSED) {
  return gen_rtx_REG (mode, PCPU_R0);
}

static bool pcpu_pass_by_reference (cumulative_args_t cum ATTRIBUTE_UNUSED, machine_mode mode, const_tree type, bool named ATTRIBUTE_UNUSED){
	unsigned HOST_WIDE_INT size;

	if (type) {
		if (AGGREGATE_TYPE_P (type))
			return true;
		size = int_size_in_bytes (type);
	} else 
		size = GET_MODE_SIZE (mode);

	return size > 2*4;
}

static bool pcpu_return_in_memory (const_tree type, const_tree fntype ATTRIBUTE_UNUSED) {
	const HOST_WIDE_INT size = int_size_in_bytes (type);
	return (size == -1 || size > 2 * UNITS_PER_WORD);
}

static rtx pcpu_function_arg (cumulative_args_t cum_v, machine_mode mode, const_tree type ATTRIBUTE_UNUSED, bool named ATTRIBUTE_UNUSED) {
  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
  // return next reg to be used for function arg
  if (*cum < 4) //idk check later
    return gen_rtx_REG (mode, *cum);
  else 
    return NULL_RTX;
}

#define PCPU_FUNCTION_ARG_SIZE(MODE, TYPE)	\
  ((MODE) != BLKmode ? GET_MODE_SIZE (MODE)	\
   : (unsigned) int_size_in_bytes (TYPE))
static void pcpu_function_arg_advance (cumulative_args_t cum_v, machine_mode mode, const_tree type, bool named ATTRIBUTE_UNUSED) {
  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
	// reserve size+1 next registers. if pointer is over avalible register use stack
  *cum = (*cum < PCPU_R4 //change to r6??
	  ? *cum + ((1 + PCPU_FUNCTION_ARG_SIZE (mode, type)) / 2)
	  : *cum);
}

static bool pcpu_function_value_regno_p (const unsigned int regno) {
  //return value in reg 0
  return (regno == PCPU_R0);
}

struct GTY(()) machine_function
{
	// Number of bytes saved on the stack for callee saved registers
	int callee_saved_reg_size;

	// Number of bytes saved on the stack for local variables
	int local_vars_size;

	// The sum of 2 sizes: local vars and padding byte for sasing the 
	// registers. Used in expand_prologue() and expand_epilogue()
	int size_for_adjusting_sp;
};

void pcpu_compute_frame(){

	int stack_alignment = 2;
	int padding_locals;
	int regno;

	/* Padding needed for each element of the frame.  */
	cfun->machine->local_vars_size = get_frame_size();

	/* Align to the stack alignment.  */
	padding_locals = cfun->machine->local_vars_size % stack_alignment;
	if (padding_locals)
		padding_locals = stack_alignment - padding_locals;
	cfun->machine->local_vars_size += padding_locals;

	cfun->machine->callee_saved_reg_size = 2*2; //return addr + old frame pointer

	for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
		if (df_regs_ever_live_p(regno) && (!call_used_regs[regno]))
			cfun->machine->callee_saved_reg_size += 2;

	cfun->machine->size_for_adjusting_sp = crtl->args.pretend_args_size + cfun->machine->local_vars_size + cfun->machine->callee_saved_reg_size + (ACCUMULATE_OUTGOING_ARGS ? (HOST_WIDE_INT) crtl->outgoing_args_size : 0);
	//cfun->machine->size_for_adjusting_sp /= 2; //16 bit memory width
}

/*
	FRAME
	[INCOMING ARGS]
	RETURN ADDR <- hard_frame_pointer 
	OLD_FRAME_POINTER
	[REGS] 
	[LOCALS] <- frame_pointer_rtx (virtual)
	[LOACLS]
	EMPTY <- sp
*/
void pcpu_expand_prologue(){
	int regno, rnpos=4;
	rtx insn;
	pcpu_compute_frame();
	if (flag_stack_usage_info)
    	current_function_static_stack_size = cfun->machine->size_for_adjusting_sp;
	insn = emit_insn(gen_movhi(hard_frame_pointer_rtx, stack_pointer_rtx));
	RTX_FRAME_RELATED_P (insn) = 1;
	insn = emit_insn(gen_movhi(gen_frame_mem(Pmode, stack_pointer_rtx), gen_rtx_REG(Pmode, PCPU_R6)));
	RTX_FRAME_RELATED_P (insn) = 1;
	insn = emit_insn(gen_movhi(gen_frame_mem(Pmode, gen_rtx_PLUS(Pmode, stack_pointer_rtx, GEN_INT(2))), hard_frame_pointer_rtx));
	RTX_FRAME_RELATED_P (insn) = 1;
	for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++) {
		if (!fixed_regs[regno] && df_regs_ever_live_p (regno) && !call_used_regs[regno]) {
			insn = emit_insn(gen_movhi(gen_frame_mem(Pmode, gen_rtx_PLUS(Pmode, stack_pointer_rtx, GEN_INT(rnpos))), gen_rtx_REG(Pmode, regno)));
			rnpos += 2;
	  		RTX_FRAME_RELATED_P (insn) = 1;
		}
	}
	insn = emit_insn(gen_addhi3(stack_pointer_rtx, stack_pointer_rtx, GEN_INT(-cfun->machine->size_for_adjusting_sp)));
	RTX_FRAME_RELATED_P (insn) = 1;
}

void pcpu_expand_epilogue(){
	int regno, rnpos=4;
	rtx insn;

	emit_insn(gen_movhi(stack_pointer_rtx, hard_frame_pointer_rtx));
    insn = emit_insn(gen_movhi(gen_rtx_REG(Pmode, PCPU_R6), gen_rtx_MEM(Pmode, stack_pointer_rtx)));
	RTX_FRAME_RELATED_P (insn) = 1;
	for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++) {
		if (!fixed_regs[regno] && df_regs_ever_live_p (regno) && !call_used_regs[regno]) {
			insn = emit_insn(gen_movhi(gen_rtx_REG(Pmode, regno), gen_rtx_MEM(Pmode, gen_rtx_PLUS(Pmode, stack_pointer_rtx, GEN_INT(rnpos)))));
			rnpos += 2;
	  		RTX_FRAME_RELATED_P (insn) = 1;
		}
	}
	emit_insn(gen_movhi(hard_frame_pointer_rtx, gen_rtx_MEM(Pmode, gen_rtx_PLUS(Pmode, stack_pointer_rtx, GEN_INT(2)))));

	emit_jump_insn(gen_returner());
}

HOST_WIDE_INT pcpu_initial_elimination_offset (int from, int to){
	HOST_WIDE_INT offset = 0;
	pcpu_compute_frame();
	if ((from) == FRAME_POINTER_REGNUM && (to) == HARD_FRAME_POINTER_REGNUM)
		offset = -cfun->machine->callee_saved_reg_size+2; //VERIFY! +2 becasue gcc begins of next addr???
	else if ((from) == ARG_POINTER_REGNUM && (to) == HARD_FRAME_POINTER_REGNUM)
   		offset = 0;
	else
		abort();

	return offset;
}

static struct machine_function* pcpu_init_machine_status (void) {
  return ggc_cleared_alloc<machine_function>();
}

static void pcpu_option_override (void) {
  /* Set the per-function-data initializer.  */
  init_machine_status = &pcpu_init_machine_status;
}


#undef  TARGET_PROMOTE_PROTOTYPES
#define TARGET_PROMOTE_PROTOTYPES	hook_bool_const_tree_true

#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P pcpu_legitimate_address_p

#undef TARGET_FUNCTION_VALUE
#define TARGET_FUNCTION_VALUE pcpu_function_value
#undef TARGET_LIBCALL_VALUE
#define TARGET_LIBCALL_VALUE pcpu_libcall_value

#undef  TARGET_RETURN_IN_MEMORY
#define TARGET_RETURN_IN_MEMORY	pcpu_return_in_memory

#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG pcpu_function_arg
#undef TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE pcpu_function_arg_advance

#undef TARGET_FUNCTION_VALUE_REGNO_P
#define TARGET_FUNCTION_VALUE_REGNO_P pcpu_function_value_regno_p

#undef  TARGET_PASS_BY_REFERENCE
#define TARGET_PASS_BY_REFERENCE pcpu_pass_by_reference

#undef  TARGET_MUST_PASS_IN_STACK
#define TARGET_MUST_PASS_IN_STACK	must_pass_in_stack_var_size

#undef TARGET_OPTION_OVERRIDE
#define TARGET_OPTION_OVERRIDE pcpu_option_override
#undef TARGET_PRINT_OPERAND
#define TARGET_PRINT_OPERAND pcpu_print_operand
#undef TARGET_PRINT_OPERAND_ADDRESS
#define TARGET_PRINT_OPERAND_ADDRESS pcpu_print_operand_address

#undef TARGET_FRAME_POINTER_REQUIRED
#define TARGET_FRAME_POINTER_REQUIRED hook_bool_void_true

struct gcc_target targetm = TARGET_INITIALIZER;

#include "gt-pcpu.h"
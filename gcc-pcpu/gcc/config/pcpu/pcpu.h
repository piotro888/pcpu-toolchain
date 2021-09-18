/* Target Definitions for moxie.
   Copyright (C) 2008-2018 Free Software Foundation, Inc.
   Contributed by Piotr Wegrzyn basing on moxie target

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_PCPU_H
#define GCC_PCPU_H

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "%{!mno-crt0:crt0%O%s} crti.o%s crtbegin.o%s"

/* Provide an ENDFILE_SPEC appropriate for svr4.  Here we tack on our own
   magical crtend.o file (see crtstuff.c) which provides part of the
   support for getting C++ file-scope static object constructed before
   entering `main', followed by the normal svr3/svr4 "finalizer" file,
   which is either `gcrtn.o' or `crtn.o'.  */

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"

/* Provide a LIB_SPEC appropriate for svr4.  Here we tack on the default
   standard C library (unless we are building a shared library) and
   the simulator BSP code.  */

// #undef LIB_SPEC
// #define LIB_SPEC "%{!shared:%{!symbolic:-lc}}"

// #undef  LINK_SPEC
// #define LINK_SPEC "%{h*} %{v:-V} %{!mel:-EB} %{mel:-EL}\
// 		   %{static:-Bstatic} %{shared:-shared} %{symbolic:-Bsymbolic}"

// #ifndef MULTILIB_DEFAULTS
// #define MULTILIB_DEFAULTS { "meb" }
// #endif

/* Layout of Source Language Data Types */

#define INT_TYPE_SIZE 16
#define SHORT_TYPE_SIZE 8
#define LONG_TYPE_SIZE 32
#define LONG_LONG_TYPE_SIZE 32

#define FLOAT_TYPE_SIZE 32
#define DOUBLE_TYPE_SIZE 64
#define LONG_DOUBLE_TYPE_SIZE 64

#define DEFAULT_SIGNED_CHAR 1

#undef  SIZE_TYPE
#define SIZE_TYPE "unsigned int"

#undef  PTRDIFF_TYPE
#define PTRDIFF_TYPE "int"

#undef  WCHAR_TYPE
#define WCHAR_TYPE "unsigned int"

#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE BITS_PER_WORD

#define REGISTER_NAMES {	\
  "$r0", "$r1",   \
  "$r2", "$r3", "$r4", "$r5",   \
  "$r6", "$r7", "V$fp", "V$cc"}

#define PCPU_R0     0
#define PCPU_R1     1 
#define PCPU_R2     2
#define PCPU_R3     3
#define PCPU_R4     4
#define PCPU_R5     5
#define PCPU_R6     6
#define PCPU_R7     7

#define PCPU_VIRT_FP 9
#define PCPU_VCC 8

#define FIRST_PSEUDO_REGISTER 10

enum reg_class
{
  NO_REGS,
  GENERAL_REGS,
  CC_REGS,
  ALL_REGS,
  LIM_REG_CLASSES
};


#define REG_CLASS_CONTENTS \
{ { 0x00000000 }, /* Empty */			   \
  { 0x000000FF },  /* GENERAL */ \
  { 0x00000300}, /* CC */ \
  { 0x000003FF }  /* All registers */              \
}

#define N_REG_CLASSES LIM_REG_CLASSES

#define REG_CLASS_NAMES {\
    "NO_REGS", \
    "GENERAL_REGS", \
    "CC_REGS", \
    "ALL_REGS" }

#define FIXED_REGISTERS     { 0, 0, 0, 0, \
			      0, 0, 0, 1, 1, 1 }

#define CALL_USED_REGISTERS { 1, 1, 1, 1, \
			      0, 1, 1, 1, 1, 1 } //auto save only r4 on calls

/* We can't copy to or from our CC register. */
#define AVOID_CCMODE_COPIES 1

/* A C expression whose value is a register class containing hard
   register REGNO.  */
#define REGNO_REG_CLASS(R) ((((R != PCPU_VCC) && (R != PCPU_VIRT_FP) )? GENERAL_REGS : CC_REGS))

/* The Overall Framework of an Assembler File */

#define ASM_COMMENT_START "#"
#define ASM_APP_ON ""
#define ASM_APP_OFF ""

#define FILE_ASM_OP     "\t.file\n"

/* Switch to the text or data segment.  */
#define TEXT_SECTION_ASM_OP  "\t.text"
#define DATA_SECTION_ASM_OP  "\t.data"

/* Assembler Commands for Alignment */

#define ASM_OUTPUT_ALIGN(STREAM,POWER) \
	fprintf (STREAM, "\t.p2align\t%d\n", POWER);

/* Output and Generation of Labels */

#define GLOBAL_ASM_OP "\t.global\t"

/* Passing Arguments in Registers */

/* A C type for declaring a variable that is used as the first
   argument of `FUNCTION_ARG' and other related values.  */
#define CUMULATIVE_ARGS unsigned int

/* If defined, the maximum amount of space required for outgoing arguments
   will be computed and placed into the variable
   `current_function_outgoing_args_size'.  No space will be pushed
   onto the stack for each call; instead, the function prologue should
   increase the stack frame size by this amount.  */
#define ACCUMULATE_OUTGOING_ARGS 1

/* A C statement (sans semicolon) for initializing the variable CUM
   for the state at the beginning of the argument list.  
   For moxie, the first arg is passed in register 2 (aka $r0).  */
#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,FNDECL,N_NAMED_ARGS) \
  (CUM = PCPU_R0)

/* How Scalar Function Values Are Returned */

/* STACK AND CALLING */

/* Define this macro if pushing a word onto the stack moves the stack
   pointer to a smaller address.  */
#define STACK_GROWS_DOWNWARD 1

/* Define this if the above stack space is to be considered part of the
   space allocated by the caller.  */
#define OUTGOING_REG_PARM_STACK_SPACE(FNTYPE) 1
// #define STACK_PARMS_IN_REG_PARM_AREA

// /* Define this if it is the responsibility of the caller to allocate
//    the area reserved for arguments passed in registers.  */
//#define REG_PARM_STACK_SPACE(FNDECL) (1 * UNITS_PER_WORD)

/* Offset from the argument pointer register to the first argument's
   address.  On some machines it may depend on the data type of the
   function.  aaaa fixed i forgot about that. offset ap = 0!!!
   emm not now. 2 makes gcc happy with sp->next free*/
#define FIRST_PARM_OFFSET(F) 2

/* for next free stack : caller argument location */
#define STACK_POINTER_OFFSET 2

/* Define this macro to nonzero value if the addresses of local variable slots
   are at negative offsets from the frame pointer.  */
#define FRAME_GROWS_DOWNWARD 1

/* Define this macro as a C expression that is nonzero for registers that are
   used by the epilogue or the return pattern.  The stack and frame
   pointer registers are already assumed to be used as needed.  */
#define EPILOGUE_USES(R) (R == PCPU_R6)

/* A C expression whose value is RTL representing the location of the
   incoming return address at the beginning of any function, before
   the prologue.  */
// #define INCOMING_RETURN_ADDR_RTX					\
//   gen_frame_mem (Pmode,							\
// 		 plus_constant (Pmode, stack_pointer_rtx, UNITS_PER_WORD))

#define INCOMING_RETURN_ADDR_RTX	gen_rtx_REG(Pmode, PCPU_R6)

// /* Describe how we implement __builtin_eh_return.  */
// #define EH_RETURN_DATA_REGNO(N)	((N) < 4 ? (N+2) : INVALID_REGNUM)

// /* Store the return handler into the call frame.  */
// #define EH_RETURN_HANDLER_RTX						\
//   gen_frame_mem (Pmode,							\
// 		 plus_constant (Pmode, frame_pointer_rtx, UNITS_PER_WORD))

/* Storage Layout */

#define BITS_BIG_ENDIAN 0
#define BYTES_BIG_ENDIAN 0
#define WORDS_BIG_ENDIAN 0

/* Alignment required for a function entry point, in bits.  */
#define FUNCTION_BOUNDARY 16

/* Define this macro as a C expression which is nonzero if accessing
   less than a word of memory (i.e. a `char' or a `short') is no
   faster than accessing a word of memory.  */
#define SLOW_BYTE_ACCESS 1

/* Number of storage units in a word; normally the size of a
   general-purpose register, a power of two from 1 or 8.  */
#define UNITS_PER_WORD 2

/* Define this macro to the minimum alignment enforced by hardware
   for the stack pointer on this machine.  The definition is a C
   expression for the desired alignment (measured in bits).  */
#define STACK_BOUNDARY 16

/* Normal alignment required for function parameters on the stack, in
   bits.  All stack parameters receive at least this much alignment
   regardless of data type.  */
#define PARM_BOUNDARY 16

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY  16

/* No data type wants to be aligned rounder than this.  */
#define BIGGEST_ALIGNMENT 16

/* The best alignment to use in cases where we have a choice.  */
#define FASTEST_ALIGNMENT 16

/* Every structures size must be a multiple of 8 bits.  */
#define STRUCTURE_SIZE_BOUNDARY 16

/* Look at the fundamental type that is used for a bit-field and use 
   that to impose alignment on the enclosing structure.
   struct s {int a:8}; should have same alignment as "int", not "char".  */
#define	PCC_BITFIELD_TYPE_MATTERS	1

/* Largest integer machine mode for structures.  If undefined, the default
   is GET_MODE_SIZE(DImode).  */
#define MAX_FIXED_MODE_SIZE 16

/* Make arrays of chars word-aligned for the same reasons.  */
// #define DATA_ALIGNMENT(TYPE, ALIGN)		\
//   (TREE_CODE (TYPE) == ARRAY_TYPE		\
//    && TYPE_MODE (TREE_TYPE (TYPE)) == QImode	\
//    && (ALIGN) < FASTEST_ALIGNMENT ? FASTEST_ALIGNMENT : (ALIGN))
     
/* Set this nonzero if move instructions will actually fail to work
   when given unaligned data.  */
#define STRICT_ALIGNMENT 1

/* Generating Code for Profiling */
#define FUNCTION_PROFILER(FILE,LABELNO) (abort (), 0)

/* Trampolines for Nested Functions.  */
#define TRAMPOLINE_SIZE (2 + 6 + 4 + 2 + 6)

/* Alignment required for trampolines, in bits.  */
#define TRAMPOLINE_ALIGNMENT 16

/* An alias for the machine mode for pointers.  */
#define Pmode         HImode

/* An alias for the machine mode used for memory references to
   functions being called, in `call' RTL expressions.  */
#define FUNCTION_MODE HImode

/* The register number of the stack pointer register, which must also
   be a fixed register according to `FIXED_REGISTERS'.  */
#define STACK_POINTER_REGNUM PCPU_R7

/* The register number of the frame pointer register, which is used to
   access automatic variables in the stack frame.  */
#define FRAME_POINTER_REGNUM PCPU_VIRT_FP

/* The register number of the arg pointer register, which is used to
   access the function's argument list.  */
#define ARG_POINTER_REGNUM PCPU_R4

#define HARD_FRAME_POINTER_REGNUM PCPU_R5

#define ELIMINABLE_REGS							\
{{ FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM },			\
 { ARG_POINTER_REGNUM,   HARD_FRAME_POINTER_REGNUM }}			

/* This macro returns the initial difference between the specified pair
   of registers.  */
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)			\
  do {									\
    (OFFSET) = pcpu_initial_elimination_offset ((FROM), (TO));		\
  } while (0)

/* A C expression that is nonzero if REGNO is the number of a hard
   register in which function arguments are sometimes passed.  */
#define FUNCTION_ARG_REGNO_P(r) (r >= PCPU_R0 && r <= PCPU_R4)

/* A macro whose definition is the name of the class to which a valid
   base register must belong.  A base register is one used in an
   address which is the register value plus a displacement.  */
#define BASE_REG_CLASS GENERAL_REGS

#define INDEX_REG_CLASS NO_REGS

#define HARD_REGNO_OK_FOR_BASE_P(NUM) \
  ((unsigned) (NUM) < FIRST_PSEUDO_REGISTER \
   && (REGNO_REG_CLASS(NUM) == GENERAL_REGS \
       || (NUM) == HARD_FRAME_POINTER_REGNUM))

/* A C expression which is nonzero if register number NUM is suitable
   for use as a base register in operand addresses.  */
#ifdef REG_OK_STRICT
#define REGNO_OK_FOR_BASE_P(NUM)		 \
  (HARD_REGNO_OK_FOR_BASE_P(NUM) 		 \
   || HARD_REGNO_OK_FOR_BASE_P(reg_renumber[(NUM)]))
#else
#define REGNO_OK_FOR_BASE_P(NUM)		 \
  ((NUM) >= FIRST_PSEUDO_REGISTER || HARD_REGNO_OK_FOR_BASE_P(NUM))
#endif

/* A C expression which is nonzero if register number NUM is suitable
   for use as an index register in operand addresses.  */
#define REGNO_OK_FOR_INDEX_P(NUM) 0

/* The maximum number of bytes that a single instruction can move
   quickly between memory and registers or between two memory
   locations.  */
#define MOVE_MAX 2

/* All load operations zero extend.  */
#define LOAD_EXTEND_OP(MEM) ZERO_EXTEND

/* A number, the maximum number of registers that can appear in a
   valid memory address.  */
#define MAX_REGS_PER_ADDRESS 1

/* An alias for a machine mode name.  This is the machine mode that
   elements of a jump-table should have.  */
#define CASE_VECTOR_MODE HImode

/* Run-time Target Specification */

#define TARGET_CPU_CPP_BUILTINS() \
  { \
    builtin_define_std ("pcpu");			\
    builtin_define_std ("PCPU");			\
  }

#define HAS_LONG_UNCOND_BRANCH true

#endif /* GCC_MOXIE_H */

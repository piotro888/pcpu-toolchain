#ifndef PCPU_H
#define PCPU_H

/*
    REGISTERS
*/

#define REGISTER_NAMES {	\
  "r0", "r1", "r2", "r3", "r4", \
  "r5", "r6", "r7" \
}

#define PCPU_R0 0
#define PCPU_R1 1
#define PCPU_R2 2
#define PCPU_R3 3
#define PCPU_R4 4
#define PCPU_R5 5
#define PCPU_R6 6
#define PCPU_R7 7

/* sp and ra fixed */
#define FIXED_REGISTERS { \
  0, 0, 0, 0, 0, 0, 0, 1 }

/* sp and ra fp ap */
#define CALL_USED_REGISTERS { \
  0, 0, 0, 0, 1, 1, 1, 1 }

// number of cpu registers 
#define FIRST_PSEUDO_REGISTER 8

/*
  REG CLASSES
*/
enum reg_class {
  NO_REGS,
  GENERAL_REGS,
  JAL_REGS,
  SP_REGS,
  ALL_REGS,
  LIM_REG_CLASSES
};

#define REG_CLASS_NAMES { \
  "NO_REGS", \
  "GENERAL_REGS", \
  "JAL_REGS", \
  "SP_REGS",  \
  "ALL_REGS" \
}

#define REG_CLASS_CONTENTS { \
  {0x00}, {0x7F}, {0x40}, {0x80}, {0xFF} \
}

/* number of reg_classes */
#define N_REG_CLASSES LIM_REG_CLASSES

#define STACK_POINTER_REGNUM PCPU_R7
#define FRAME_POINTER_REGNUM PCPU_R5//can be eliminated ! read docs elimination and frame register
#define ARG_POINTER_REGNUM PCPU_R4

// registers where parameters can be stored
#define FUNCTION_ARG_REGNO_P(r) (r >= PCPU_R0 && r <= PCPU_R5)

#define BASE_REG_CLASS GENERAL_REGS

#define REGNO_OK_FOR_BASE_P(r) (r >= PCPU_R0 && r <= PCPU_R7) // if ok in address

#define REGNO_REG_CLASS(R) ((R < PCPU_R6) ? GENERAL_REGS :		\
                            (R == PCPU_R6 ? JAL_REGS : SP_REGS))

/* SIZES */

/* Word - 32bit - 2*16bit min. addressable unit */
#define UNITS_PER_WORD 2

/* Maximum number of bytes moved between reg-mem in one instruction */
#define MOVE_MAX 2

/* 16 bit function address pointer pointer */
// CHANGE LATER TO 32 !!! TODO
#define FUNCTION_MODE HImode

/* endianess */
/* bit ops count from lsb */
#define BITS_BIG_ENDIAN 0 
/* little endian target */
#define BYTES_BIG_ENDIAN 0
#define WORDS_BIG_ENDIAN 0

/* 16 bit memory address pointer */
// CHANGE LATER TO 32 !!! TODO
#define Pmode HImode

/* PC/jmp MODE 16bit */
#define CASE_VECTOR_MODE HImode

#define DEFAULT_SIGNED_CHAR 0

//copied from avr idk if neccessary
#define INT_TYPE_SIZE 16
#define SHORT_TYPE_SIZE 8
#define LONG_TYPE_SIZE 32
#define LONG_LONG_TYPE_SIZE 32
//float must be set to 32bit, otherwise gcc fails to build (ieee std)
#define FLOAT_TYPE_SIZE 32 
#define DOUBLE_TYPE_SIZE 64
#define LONG_DOUBLE_TYPE_SIZE 32

/* MEMORY */
/* address may contain max 1 reg */
#define MAX_REGS_PER_ADDRESS 1
/* accessing byte is not faster than word */
#define SLOW_BYTE_ACCESS 1

/* ALIGN */
/* can't access not aligned data by normal instr */
#define STRICT_ALIGNMENT 1
/* function enter align */
#define FUNCTION_BOUNDARY 32
/* stack align */
#define STACK_BOUNDARY 16
/* no needed for bigger align for data types */
#define BIGGEST_ALIGNMENT 16 // OR 32???
// function parameters on stack
#define PARM_BOUNDARY 16
/* parameter pointer points to first arg */
#define FIRST_PARM_OFFSET(F) 0

/* ASM */
/* object labels */
// #define LOCAL_LABEL_PREFIX "."
// #define ASM_GENERATE_INTERNAL_LABEL(LABEL,PREFIX,NUM)			\
//   sprintf ((LABEL), "*%s%s%ld", (LOCAL_LABEL_PREFIX), (PREFIX), (long)(NUM))
#define ASM_OUTPUT_ALIGN(STREAM,LOG)					\
  fprintf (STREAM, "\t.align\t%d\n", (LOG))
#define ASM_COMMENT_START ";"
#define ASM_APP_ON ""
#define ASM_APP_OFF ""
#define TEXT_SECTION_ASM_OP  "\t.text"
#define DATA_SECTION_ASM_OP  "\t.data"
#define GLOBAL_ASM_OP "\t.global\t"
//#define READONLY_DATA_SECTION_ASM_OP	"\t.rodata"
#define BSS_SECTION_ASM_OP	"\t.bss"


/* OTHER */

/* Type to store info about function call used in compiler */
#define CUMULATIVE_ARGS unsigned int
#define STACK_GROWS_DOWNWARD 1

// this was function hook aaaaaaaaaaaa
//#define TARGET_FRAME_POINTER_REQUIRED false

/* pairs of possible substitution */
#define ELIMINABLE_REGS							\
{{ ARG_POINTER_REGNUM,   STACK_POINTER_REGNUM}, \
 { ARG_POINTER_REGNUM,   FRAME_POINTER_REGNUM},	\
 { FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}}//,	
// { FRAME_POINTER_REGNUM, ARG_POINTER_REGNUM}}		

#define TRAMPOLINE_SIZE 16 //idk riscv sets that

//verify later! idk moxie
#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,FNDECL,N_NAMED_ARGS) \
  (CUM = PCPU_R0) 
#endif

/* compile time defines -> pcpu __pcpu __pcpu__*/
#define TARGET_CPU_CPP_BUILTINS() builtin_define_std ("pcpu"); 
/* no function profiler */
#define FUNCTION_PROFILER(FILE,LABELNO) (abort (), 0)
/* everywhere set to zero / no index reg ?*/
#define REGNO_OK_FOR_INDEX_P(REGNO) 0
#define INDEX_REG_CLASS NO_REGS

/* TMP : IMPLEMENT LATER!! */
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFF) (OFF)=3
// #define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
// ( fputs ("\t.common ", (FILE)),		\
//   assemble_name ((FILE), (NAME)),		\
//   fprintf ((FILE), "," HOST_WIDE_INT_PRINT_UNSIGNED",\"bss\"\n", (SIZE)))
// #define ASM_OUTPUT_SKIP(FILE,SIZE)  \
//   fprintf (FILE, "\t.skip " HOST_WIDE_INT_PRINT_UNSIGNED"\n", (SIZE))
// #define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ALIGNED)		\
// ( fputs ("\t.reserve ", (FILE)),					\
//   assemble_name ((FILE), (NAME)),					\
//   fprintf ((FILE), "," HOST_WIDE_INT_PRINT_UNSIGNED",\"bss\",%u\n",	\
// 	   (SIZE), ((ALIGNED) / BITS_PER_UNIT)))
// #define DBX_DEBUGGING_INFO

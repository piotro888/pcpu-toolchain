#ifndef PCPU_H
#define PCPU_H
#define dummy_cond
/*
    REGISTERS
*/

#define REGISTER_NAMES {	\
  "r0", "r1", "r2", "r3", "r4" \
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

/* MEMORY */
/* address may contain max 1 reg */
#define MAX_REGS_PER_ADDRESS 1

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

/* OTHER */

/* Type to store info about function call used in compiler */
#define CUMULATIVE_ARGS int

#define TARGET_FRAME_POINTER_REQUIRED false

/* pairs of possible substitution */
#define ELIMINABLE_REGS							\
{{ ARG_POINTER_REGNUM,   STACK_POINTER_REGNUM}, \
 { ARG_POINTER_REGNUM,   FRAME_POINTER_REGNUM},	\
 { FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},	\
 { FRAME_POINTER_REGNUM, ARG_POINTER_REGNUM}}		\

#define TRAMPOLINE_SIZE 16 //idk riscv sets that

//verify later! idk moxie
#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,FNDECL,N_NAMED_ARGS) \
  (CUM = PCPU_R0) 
#endif
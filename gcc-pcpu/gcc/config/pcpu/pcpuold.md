(define_attr "length" "" (const_int 2))

;; MOV 
;; HI mode - 16 bit

(define_expand "std"
  [(set (match_operand:HI 0 "general_operand" "")
	(match_operand:HI 1 "general_operand" ""))]
  ""
  "
{
  /* If this is a store, force the value into a register.  */
  if (MEM_P (operands[0]))
    operands[1] = force_reg (HImode, operands[1]);
}")

(define_insn "movhi"
    [(set (match_operand:HI 0 "register_operand" "=r")
            (match_operand:HI 1 "register_operand" "r"))]
    ""
    "mov %0, %1"
)

(define_insn "*std"
    [(set (match_operand:HI 0 "memory_operand" "=m")
            (match_operand:HI 1 "register_operand" "r"))]
    ""
    "std %0, %1"
)
;;(define_insn "stdfake"
    ;;[(set (match_operand:HI 0 "memory_operand" "=m")
     ;;       (match_operand:HI 1 "general_operand" ""))]
   ;; ""
  ;;  "std %0, %1"
;;)

(define_insn "ldi"
    [(set (match_operand:HI 0 "register_operand" "=r")
            (match_operand:HI 1 "immediate_operand" "i"))]
    ""
    "ldi %0, %1"
)

(define_insn "ldd"
    [(set (match_operand:HI 0 "register_operand" "=r")
            (match_operand:HI 1 "memory_operand" "m"))]
    ""
    "ldd %0, %1"
)

;; dummy condition to compile

(define_insn "nop"
    [(const_int 0)]
    ""
    "nop"
)

(define_insn "jump"
	[(set (pc)
	(label_ref(match_operand 0 "" "")))]
	""
	"jmp\\t%1")

(define_insn "returner"
    [(return)]
    "reload_completed"
    "ret"
)
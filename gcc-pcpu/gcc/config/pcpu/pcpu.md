(define_attr "length" "" (const_int 2))

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
)

(define_expand "movhi"
  [(set (match_operand:HI 0 "general_operand" "")
 	(match_operand:HI 1 "general_operand" ""))]
  ""
  "{
    if(GET_CODE(operands[0]) == MEM) {
      operands[1] = force_reg (HImode, operands[1]);
    }
  }"
)

(define_insn "*movhi"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r,r,m,r")
   (match_operand:HI 1 "general_operand" "r,i,r,m"))]
  ""
  "@
  mov %0, %1
  ldi %0, %1
  sto %1, %0, 0
  ldo %0, %1, 0"
)

(define_insn "addhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r")
    (plus:HI
    (match_operand:HI 1 "general_operand" "r")
    (match_operand:HI 2 "general_operand" "r")))]
  ""
  "add %0, %1, %2"
)

(define_insn "jump"
	[(set (pc)
	(label_ref(match_operand 0 "" "")))]
	""
	"jmp %1")

(define_insn "returner"
	[(return)]
	"reload_completed"
	"ret")
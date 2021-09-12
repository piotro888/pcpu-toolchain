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
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r,r")
    (plus:HI
    (match_operand:HI 1 "general_operand" "r,r")
    (match_operand:HI 2 "general_operand" "r,i")))]
  ""
  "@
  add %0, %1, %2
  adi %0, %1, %2"
)

;; CALL AND JUMPS

(define_insn "jump"
	[(set (pc)
	(label_ref(match_operand 0 "" "")))]
	""
	"jmp %1")

(define_expand "call_value"
  [(set (match_operand 0 "" "")
		(call (match_operand:HI 1 "memory_operand" "")
		 (match_operand 2 "" "")))]
  ""
{
  gcc_assert (MEM_P (operands[1]));
}) ;; call and set register

(define_insn "*call_value"
  [(set (match_operand 0 "register_operand" "=r")
	(call (mem:HI (match_operand:HI
		       1 "general_operand" "i"))
	      (match_operand 2 "" "")))]
    ""
    "jal r6, %1"
)


(define_expand "call"
  [(call (match_operand:HI 0 "memory_operand" "")
		(match_operand 1 "general_operand" ""))
    ]
  ""{}) ;; call not wroking without this

(define_insn "*call"
  [(call (mem:HI (match_operand:HI
		  0 "nonmemory_operand" "i"))
	 (match_operand 1 "" ""))]
    ""
    "jal r6, %0"
)


(define_expand "prologue"
  [(return)]
  "" {
    pcpu_expand_prologue();
    DONE;}
  )

(define_expand "epilogue"
  [(return)]
  "" {
    pcpu_expand_epilogue();
    DONE;}
  )

(define_insn "returner"
  [(return)]
  "reload_completed"
  "srs r6, 0")
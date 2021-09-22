(define_attr "length" "" (const_int 2))

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
)

;; ----------------------------
;; MOVES
;; ----------------------------

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
  sto %1, %0
  ldo %0, %1"
)

(define_expand "movqi"
  [(set (match_operand:QI 0 "general_operand" "")
 	(match_operand:QI 1 "general_operand" ""))]
  ""
  "{
    if(GET_CODE(operands[0]) == MEM) {
      operands[1] = force_reg (QImode, operands[1]);
    }
  }"
)

(define_insn "*movqi"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=r,r,m,r")
    (match_operand:QI 1 "general_operand" "r,i,r,m"))]
  ""
  "@
  mov %0, %1
  ldi %0, %1
  sto**8 %1, %0
  ldo**8 %0, %1")

(define_insn "zero_extendqihi2"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(zero_extend:HI (match_operand:QI 1 "nonimmediate_operand" "0")))]
  ""
  "ani %0, %0, 0xFF"
  [(set_attr "length" "2")])

(define_insn "extendqihi2"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(sign_extend:HI (match_operand:QI 1 "nonimmediate_operand" "0")))]
  ""
  "gcc**extsgn %0"
  [(set_attr "length" "2")])

;; ----------------------------
;; ARTIHMETIC
;; ----------------------------

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
(define_insn "subhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r")
    (minus:HI
    (match_operand:HI 1 "general_operand" "r")
    (match_operand:HI 2 "general_operand" "r")))]
  ""
  "sub %0, %1, %2"
)

(define_insn "udivhi3"
  [(set (match_operand:HI 0 "register_operand" "=r")
	  (udiv:HI
	   (match_operand:HI 1 "register_operand" "r")
	   (match_operand:HI 2 "register_operand" "r")))]
  ""
  "div %0, %1, %2")

  (define_insn "mulhi3"
  [(set (match_operand:HI 0 "register_operand" "=r")
	  (mult:HI
	   (match_operand:HI 1 "register_operand" "r")
	   (match_operand:HI 2 "register_operand" "r")))]
  ""
  "mul %0, %1, %2")


;; ----------------------------
;; LOGIC
;; ---------------------------- 

(define_insn "andhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r,r")
    (and:HI
    (match_operand:HI 1 "general_operand" "r,r")
    (match_operand:HI 2 "general_operand" "r,i")))]
  ""
  "@
  and %0, %1, %2
  ani %0, %1, %2"
)

(define_insn "orhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r,r")
    (ior:HI
    (match_operand:HI 1 "general_operand" "r,r")
    (match_operand:HI 2 "general_operand" "r,i")))]
  ""
  "@
  orr %0, %1, %2
  ori %0, %1, %2"
)

(define_insn "xorhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r,r")
    (xor:HI
    (match_operand:HI 1 "general_operand" "r,r")
    (match_operand:HI 2 "general_operand" "r,i")))]
  ""
  "@
  xor %0, %1, %2
  xoi %0, %1, %2"
)

(define_insn "one_cmplhi2"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r")
    (not:HI
    (match_operand:HI 1 "general_operand" "r")))]
  ""
  "not %0, %1"
)

(define_insn "lshrhi3"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(lshiftrt:HI (match_operand:HI 1 "register_operand" "r")
		     (match_operand:HI 2 "register_operand" "r")))]
  ""
  "shr %0, %1, %2")

(define_insn "ashrhi3"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(ashiftrt:HI (match_operand:HI 1 "register_operand" "0")
		     (match_operand:HI 2 "immediate_operand" "i")))
         (clobber (const_int 4))]
  ""
  "a**shr %0, %2")

(define_insn "ashlhi3"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(ashift:HI (match_operand:HI 1 "register_operand" "r")
		     (match_operand:HI 2 "register_operand" "r")))]
  ""
  "shl %0, %1, %2")


;; ----------------------------
;; COMPARE
;; ----------------------------

(define_constants [(CC_REG 8)])

(define_expand "cbranchhi4"
  [(set (reg:CC CC_REG)
        (compare:CC
         (match_operand:HI 1 "general_operand" "")
         (match_operand:HI 2 "general_operand" "")))
   (set (pc)
        (if_then_else (match_operator 0 "comparison_operator"
                       [(reg:CC CC_REG) (const_int 0)])
                      (label_ref (match_operand 3 "" ""))
                      (pc)))]
  ""
  "
  /* Force the compare operands into registers.  */
  if (GET_CODE (operands[1]) != REG)
	operands[1] = force_reg (HImode, operands[1]);
  if (GET_CODE (operands[2]) != REG && (GET_CODE (operands[2]) != CONST))
	operands[2] = force_reg (HImode, operands[2]);
  "
)

(define_insn "*cmphi"
  [(set (reg:CC CC_REG)
	(compare
	 (match_operand:HI 0 "register_operand" "r,r")
	 (match_operand:HI 1 "nonmemory_operand"	"r,i")))]
  ""
  "@
  cmp %0, %1
  cmi %0, %1")


;; ----------------------------
;; CONDITIONAL JUMPS
;; ----------------------------

(define_code_iterator cond [ne eq lt gt ge le gtu ltu geu leu])
(define_code_attr CC [(ne "ne") (eq "eq") (lt "lt") 
		      (gt "gt")  (ge "ge") (le "le") (gtu "gt**aa") (ltu "ca") (geu "ge**az") (leu "**ce")])

(define_insn "*b<cond:code>"
  [(set (pc)
	(if_then_else (cond (reg:CC CC_REG)
			    (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "j<CC> %l0"
)

;; unsigned variants

(define_insn "*bltu"[(set (pc) (if_then_else (cond(reg:CC CC_REG) (const_int 0)) (match_operand 0 "" "") (pc)))] ""
  "jca %0"
)

(define_insn "*bleu"[(set (pc) (if_then_else (cond(reg:CC CC_REG) (const_int 0)) (match_operand 0 "" "") (pc)))] ""
  "jca %0\njeq %0"
  [(set_attr "length"	"4")]
)

(define_insn "*bgeu"[(set (pc) (if_then_else (cond(reg:CC CC_REG) (const_int 0)) (match_operand 0 "" "") (pc)))] ""
  "j**geu %0"
)

(define_insn "*bgtu"[(set (pc) (if_then_else (cond(reg:CC CC_REG) (const_int 0)) (match_operand 0 "" "") (pc)))] ""
  "j**gtu %0"
)

;; ----------------------------
;; CALL AND JUMPS
;; ----------------------------

(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  "jmp %l0")

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
		       1 "immediate_operand" "i"))
	      (match_operand 2 "" "")))]
    ""
    "jal r6, %1"
)

;;VERIFY
(define_insn "*call_value_indirect"
  [(set (match_operand 0 "register_operand" "=r")
	(call (mem:HI (match_operand:HI
		       1 "register_operand" "r"))
	      (match_operand 2 "" "")))]
  ""
  "srs %1, 0"
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
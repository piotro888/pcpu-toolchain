;; MOV 
;; HI mode - 16 bit

(define_insn "mov"
    [(set (match_operand:HI 0 "register_operand" "=r")
            (match_operand:HI 1 "register_operand" "r"))]
    "true"
    "mov %0, %1"
    )
;; dummy condition to compile

(define_insn "nop"
    [(const_int 0)]
    ""
    "nop"
)

(define_attr "length" "" (const_int 2))

;; MOV 
;; HI mode - 16 bit

(define_insn "movhi"
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
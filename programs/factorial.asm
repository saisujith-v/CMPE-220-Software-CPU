; factorial.asm - factorial using stack + two phases
; R1 = n, R2 = result, R3 = temp, R4 = depth, R5 = SP, R6 = 1

    LOADI R1, #5        ; n = 5 (compute 5!)
    LOADI R2, #1        ; result = 1
    LOADI R4, #0        ; depth = 0 (recursion depth counter)
    LOADI R5, #192      ; stack pointer = 192 (0x00C0)
    LOADI R6, #1        ; constant 1 (for comparisons)
    BRANCH push_phase   ; jump to push phase

; ---- PUSH PHASE: recursive descent ----
; while (n > 1) { push n, depth++, n-- }
push_phase:
    SUB R7, R1, R6      ; R7 = n - 1
    BEQ pop_phase       ; if n == 1, done descending, go to pop
    
    STORE R1, [R5]      ; stack[SP] = n (save current n)
    ADDI R5, #2         ; SP += 2 (move stack pointer, word = 2 bytes)
    ADDI R4, #1         ; depth++ (track recursion depth)
    SUBI R1, #1         ; n-- (decrement for next "recursive call")
    BRANCH push_phase   ; loop back

; ---- POP PHASE: recursive unwind ----
; while (depth > 0) { result *= stack[--SP], depth-- }
pop_phase:
    LOADI R7, #0        
    SUB R7, R4, R7      ; check if depth == 0
    BEQ done            ; if depth == 0, all done
    
    SUBI R5, #2         ; SP -= 2 (move stack pointer back)
    LOAD R3, [R5]       ; temp = stack[SP] (load saved value)
    
    ; Multiply: result *= temp (using repeated addition)
    LOADI R1, #0
    ADD R1, R2, R1      ; R1 = old_result (save current result)
    LOADI R2, #0        ; result = 0 (reset)
mul_loop:
    LOADI R7, #0
    SUB R7, R3, R7      ; check if temp == 0
    BEQ mul_done        ; if temp == 0, multiplication done
    ADD R2, R2, R1      ; result += old_result
    SUBI R3, #1         ; temp-- (decrement counter)
    BRANCH mul_loop     ; loop
mul_done:
    SUBI R4, #1         ; depth-- (unwind one level)
    BRANCH pop_phase    ; loop back to pop next value

; ---- DONE: store result ----
done:
    LOADI R5, #1        ; R5 = 1
    ADD R5, R5, R5      ; R5 = 2
    ADD R5, R5, R5      ; R5 = 4
    ADD R5, R5, R5      ; R5 = 8
    ADD R5, R5, R5      ; R5 = 16
    ADD R5, R5, R5      ; R5 = 32
    ADD R5, R5, R5      ; R5 = 64
    ADD R5, R5, R5      ; R5 = 128
    ADD R5, R5, R5      ; R5 = 256 (0x0100)
    STORE R2, [R5]      ; Memory[0x0100] = result
    HALT                ; end program, R2 holds 5! = 120

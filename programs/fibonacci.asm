; ============================================================================
; SIMPLIFIED FIBONACCI PROGRAM
; ============================================================================
; Generates the first 10 Fibonacci numbers and stores them in memory.
; Based on the reference logic provided.
;
; Registers:
; R1 = Previous term
; R2 = Current term
; R3 = Loop counter
; R4 = Limit (10)
; R5 = Memory Pointer (starts at 0x0080)
; R6 = Temp (Sum)
; R7 = Constant 0 (Helper for moves)
; ============================================================================

; Initialize Registers
LOADI R7, #0        ; R7 = 0 (Constant)
LOADI R1, #0        ; Prev = 0
LOADI R2, #1        ; Curr = 1
LOADI R3, #0        ; Counter = 0
LOADI R4, #10       ; Limit = 10
LOADI R5, #128      ; MemPtr = 128 (0x0080)

loop:
    ; Compare Counter vs Limit
    SUB R6, R3, R4      ; R6 = Counter - Limit
    BEQ end             ; If Counter == Limit, exit

    ; Compute Next Term
    ADD R6, R1, R2      ; Temp = Prev + Curr

    ; Shift Terms
    ADD R1, R2, R7      ; Prev = Curr (R2 + 0)
    ADD R2, R6, R7      ; Curr = Temp (R6 + 0)

    ; Store to Memory
    STORE R2, [R5]      ; Mem[MemPtr] = Curr

    ; Increment Pointers
    ADDI R5, #2         ; MemPtr += 2 (Move to next word)
    ADDI R3, #1         ; Counter += 1

    BRANCH loop         ; Repeat

end:
    HALT

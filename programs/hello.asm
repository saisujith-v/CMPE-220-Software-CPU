; ============================================================================
; SIMPLIFIED HELLO WORLD PROGRAM
; ============================================================================
; Stores the ASCII characters "HELLO" into memory.
;
; Registers:
; R1 = Memory Pointer (starts at 0x0200)
; R2 = Character value
; ============================================================================

; Initialize Memory Pointer
LOADI R1, #1        ; R1 = 1
ADD R1, R1, R1      ; R1 = 2
ADD R1, R1, R1      ; R1 = 4
ADD R1, R1, R1      ; R1 = 8
ADD R1, R1, R1      ; R1 = 16
ADD R1, R1, R1      ; R1 = 32
ADD R1, R1, R1      ; R1 = 64
ADD R1, R1, R1      ; R1 = 128
ADD R1, R1, R1      ; R1 = 256
ADD R1, R1, R1      ; R1 = 512 (0x0200)

; Store 'H' (72)
LOADI R2, #72
STORE R2, [R1]
ADDI R1, #2

; Store 'E' (69)
LOADI R2, #69
STORE R2, [R1]
ADDI R1, #2

; Store 'L' (76)
LOADI R2, #76
STORE R2, [R1]
ADDI R1, #2

; Store 'L' (76)
STORE R2, [R1]      ; 'L' is already in R2
ADDI R1, #2

; Store 'O' (79)
LOADI R2, #79
STORE R2, [R1]

HALT

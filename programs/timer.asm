; ============================================================================
; TIMER PROGRAM WITH DELAY
; ============================================================================
; Counts down from 10 to 0 with 1-second delay.
; Stores values in memory at 0x00A0.
;
; Registers:
; R1 = Counter (10)
; R2 = Memory Pointer (0x00A0)
; R3 = Target Time
; R4 = Current Time
; R5 = Delay (1000 ms)
; R6 = Timer I/O Address (0xF003)
; R7 = Temp
; ============================================================================

; Initialize Counter and Pointer
LOADI R1, #10       ; Counter = 10
LOADI R2, #160      ; MemPtr = 160 (0x00A0)

; Build Delay (1000)
LOADI R5, #250
ADD R5, R5, R5      ; 500
ADD R5, R5, R5      ; 1000

; Build Timer Address (0xF003)
LOADI R6, #240      ; 0x00F0
ADD R6, R6, R6      ; 0x01E0
ADD R6, R6, R6      ; 0x03C0
ADD R6, R6, R6      ; 0x0780
ADD R6, R6, R6      ; 0x0F00
ADD R6, R6, R6      ; 0x1E00
ADD R6, R6, R6      ; 0x3C00
ADD R6, R6, R6      ; 0x7800
ADD R6, R6, R6      ; 0xF000
ADDI R6, #3         ; 0xF003

loop:
    ; Store current counter
    STORE R1, [R2]
    ADDI R2, #2

    ; Check if done (Counter == 0)
    LOADI R7, #0
    SUB R7, R1, R7
    BEQ end

    ; Decrement Counter
    LOADI R7, #1
    SUB R1, R1, R7

    ; --- DELAY LOGIC ---
    ; Get Start Time
    LOAD R3, [R6]       ; R3 = Current Time
    ADD R3, R3, R5      ; R3 = Target Time (Current + 1000)

wait_loop:
    LOAD R4, [R6]       ; R4 = Current Time
    SUB R7, R4, R3      ; R7 = Current - Target
    ; If Current < Target, result is negative (MSB set). 
    ; If Current >= Target, result is positive or zero.
    ; We wait while Current < Target.
    ; BLT checks if Rs1 < Rs2.
    ; Let's use BLT: BLT Target, Current (if Target < Current, we are done?)
    ; No, we want to loop while Current < Target.
    ; BLT R4, R3, wait_loop
    
    ; Check if R4 (Current) < R3 (Target)
    ; SUB R7, R4, R3
    ; If R4 < R3, result is negative (N flag set).
    ; BLT jumps if N flag is set.
    BLT wait_loop

    BRANCH loop

end:
    HALT

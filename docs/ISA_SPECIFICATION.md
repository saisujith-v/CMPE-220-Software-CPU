# Instruction Set Architecture (ISA) Specification

## Overview

This document provides a complete specification of the Software CPU's Instruction Set Architecture (ISA).

## General Information

- **Architecture**: 16-bit RISC-like
- **Instruction Length**: Fixed 16-bit
- **Endianness**: Little-endian
- **Word Size**: 16 bits (2 bytes)
- **Address Space**: 64KB (0x0000 - 0xFFFF)

## Register Set

### General Purpose Registers
- **R0 - R7**: Eight 16-bit general-purpose registers
- Can be used for any operation
- No special restrictions

### Special Purpose Registers
- **PC (Program Counter)**: 16-bit, points to next instruction
- **SP (Stack Pointer)**: 16-bit, points to top of stack
- **IR (Instruction Register)**: 16-bit, holds current instruction
- **FLAGS**: 8-bit status register

### Status Flags

| Bit | Flag | Name | Description |
|-----|------|------|-------------|
| 0 | Z | Zero | Set when result is zero |
| 1 | N | Negative | Set when result MSB is 1 |
| 2 | C | Carry | Set on unsigned overflow |
| 3 | O | Overflow | Set on signed overflow |
| 4-7 | - | Reserved | Unused |

## Instruction Formats

### Format 1: Register-Register Operations
```
 15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│  Opcode   │   Rd      │   Rs1     │   Rs2     │   Unused      │
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
```

**Used by**: ADD, SUB, AND, OR, XOR

**Example**: `ADD R1, R2, R3` → R1 = R2 + R3

### Format 2: Register-Immediate Operations
```
 15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│  Opcode   │   Rd      │        Immediate (9-bit signed)       │
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
```

**Used by**: ADDI, SUBI, LOADI

**Immediate Range**: -256 to +255 (9-bit signed)

**Example**: `ADDI R1, R2, #10` → R1 = R2 + 10

### Format 3: Memory Operations
```
 15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│  Opcode   │   Rd      │   Rs      │   Offset (6-bit signed)   │
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
```

**Used by**: LOAD, STORE

**Offset Range**: -32 to +31 (6-bit signed)

**Example**: `LOAD R1, [R2 + 4]` → R1 = Memory[R2 + 4]

### Format 4: Branch Operations
```
 15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│  Opcode   │            Offset (12-bit signed)                 │
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
```

**Used by**: BRANCH, BEQ, BNE, BLT

**Offset Range**: -2048 to +2047 (12-bit signed, word-aligned)

**Example**: `BEQ loop` → if (Z flag) PC = PC + offset

## Instruction Set

### Arithmetic Instructions

#### ADD - Add Registers
```
Syntax:   ADD Rd, Rs1, Rs2
Opcode:   0x1
Format:   Register-Register
Operation: Rd ← Rs1 + Rs2
Flags:    Z, N, C
Example:  ADD R0, R1, R2
```

#### ADDI - Add Immediate
```
Syntax:   ADDI Rd, Rs, Imm
Opcode:   0x2
Format:   Register-Immediate
Operation: Rd ← Rs + Imm
Flags:    Z, N
Example:  ADDI R0, R1, #10
```

#### SUB - Subtract Registers
```
Syntax:   SUB Rd, Rs1, Rs2
Opcode:   0x3
Format:   Register-Register
Operation: Rd ← Rs1 - Rs2
Flags:    Z, N
Example:  SUB R0, R1, R2
```

#### SUBI - Subtract Immediate
```
Syntax:   SUBI Rd, Rs, Imm
Opcode:   0x4
Format:   Register-Immediate
Operation: Rd ← Rs - Imm
Flags:    Z, N
Example:  SUBI R0, R1, #5
```

### Logical Instructions

#### AND - Bitwise AND
```
Syntax:   AND Rd, Rs1, Rs2
Opcode:   0x5
Format:   Register-Register
Operation: Rd ← Rs1 & Rs2
Flags:    Z, N
Example:  AND R0, R1, R2
```

#### OR - Bitwise OR
```
Syntax:   OR Rd, Rs1, Rs2
Opcode:   0x6
Format:   Register-Register
Operation: Rd ← Rs1 | Rs2
Flags:    Z, N
Example:  OR R0, R1, R2
```

#### XOR - Bitwise XOR
```
Syntax:   XOR Rd, Rs1, Rs2
Opcode:   0x7
Format:   Register-Register
Operation: Rd ← Rs1 ^ Rs2
Flags:    Z, N
Example:  XOR R0, R1, R2
```

### Memory Instructions

#### LOAD - Load from Memory
```
Syntax:   LOAD Rd, [Rs + offset]
Opcode:   0x8
Format:   Memory
Operation: Rd ← Memory[Rs + offset]
Flags:    None
Example:  LOAD R0, [R1]
          LOAD R0, [R1 + 4]
```

#### STORE - Store to Memory
```
Syntax:   STORE Rd, [Rs + offset]
Opcode:   0x9
Format:   Memory
Operation: Memory[Rs + offset] ← Rd
Flags:    None
Example:  STORE R0, [R1]
          STORE R0, [R1 + 4]
```

#### LOADI - Load Immediate
```
Syntax:   LOADI Rd, Imm
Opcode:   0xA
Format:   Register-Immediate
Operation: Rd ← Imm
Flags:    None
Example:  LOADI R0, #42
          LOADI R1, #0xFF
```

### Control Flow Instructions

#### BRANCH - Unconditional Branch
```
Syntax:   BRANCH offset
          B offset
Opcode:   0xB
Format:   Branch
Operation: PC ← PC + offset
Flags:    None
Example:  BRANCH loop
          B start
```

#### BEQ - Branch if Equal
```
Syntax:   BEQ offset
Opcode:   0xC
Format:   Branch
Operation: if (Z == 1) PC ← PC + offset
Flags:    None (reads Z)
Example:  BEQ equal_label
```

#### BNE - Branch if Not Equal
```
Syntax:   BNE offset
Opcode:   0xD
Format:   Branch
Operation: if (Z == 0) PC ← PC + offset
Flags:    None (reads Z)
Example:  BNE loop
```

#### BLT - Branch if Less Than
```
Syntax:   BLT offset
Opcode:   0xE
Format:   Branch
Operation: if (N == 1) PC ← PC + offset
Flags:    None (reads N)
Example:  BLT negative_case
```

### Special Instructions

#### NOP - No Operation
```
Syntax:   NOP
Opcode:   0x0
Format:   -
Operation: (none)
Flags:    None
Example:  NOP
```

#### HALT - Halt Execution
```
Syntax:   HALT
Opcode:   0xF
Format:   -
Operation: Stop execution
Flags:    None
Example:  HALT
```

## Addressing Modes

### 1. Register Direct
```
ADD R1, R2, R3
```
Operands are in registers.

### 2. Immediate
```
ADDI R1, R2, #10
```
One operand is a constant value.

### 3. Register Indirect
```
LOAD R1, [R2]
```
Address is in a register.

### 4. Indexed (Base + Offset)
```
LOAD R1, [R2 + 4]
```
Address = register + offset.

### 5. PC-Relative
```
BRANCH loop
```
Target address = PC + offset.

## Memory Map

```
Address Range    | Size    | Description
-----------------|---------|---------------------------
0x0000 - 0xEFFF  | 60 KB   | RAM (Program and Data)
0xF000 - 0xF0FF  | 256 B   | Memory-Mapped I/O
0xF100 - 0xFFFF  | 3.75 KB | Stack (grows downward)
```

### Memory-Mapped I/O Addresses

| Address | Name | Access | Description |
|---------|------|--------|-------------|
| 0xF000 | CONSOLE_OUT | Write | Console output (write char) |
| 0xF001 | CONSOLE_IN | Read | Console input (read char) |
| 0xF002 | TIMER_CTRL | Write | Timer control (0=off, 1=on) |
| 0xF003 | TIMER_VAL | R/W | Timer value |

## Instruction Encoding Examples

### Example 1: ADD R1, R2, R3
```
Binary:   0001 001 010 011 000
Hex:      0x1290
Fields:   Opcode=1, Rd=1, Rs1=2, Rs2=3
```

### Example 2: LOADI R0, #42
```
Binary:   1010 000 000101010
Hex:      0xA02A
Fields:   Opcode=A, Rd=0, Imm=42
```

### Example 3: BRANCH -4
```
Binary:   1011 111111111100
Hex:      0xBFFC
Fields:   Opcode=B, Offset=-4 (two's complement)
```

## Assembly Language Syntax

### Comments
```assembly
; This is a comment
NOP  ; This is also a comment
```

### Labels
```assembly
start:          ; Label definition
    LOADI R0, #1
loop:           ; Another label
    ADDI R0, R0, #1
    BRANCH loop
```

### Numeric Literals
```assembly
LOADI R0, #10      ; Decimal
LOADI R0, #0x0A    ; Hexadecimal
LOADI R0, #0xF000  ; Hex address
```

### Instruction Examples
```assembly
; Arithmetic
ADD R0, R1, R2
ADDI R0, R0, #1
SUB R3, R4, R5
SUBI R3, R3, #10

; Logical
AND R0, R1, R2
OR R0, R1, R2
XOR R0, R1, R2

; Memory
LOAD R0, [R1]
LOAD R0, [R1 + 4]
STORE R0, [R2]
STORE R0, [R2 + 8]

; Control flow
BRANCH loop
BEQ equal
BNE not_equal
BLT less_than

; Special
NOP
HALT
```

## Execution Cycle

### Fetch Phase
1. Read instruction from Memory[PC]
2. Store in IR
3. Increment PC by 2

### Decode Phase
1. Extract opcode from IR[15:12]
2. Extract operands based on format
3. Sign-extend immediates if needed

### Execute Phase
1. Perform operation
2. Update destination register
3. Update flags if applicable
4. Handle branches (modify PC)

## Programming Examples

### Example 1: Simple Addition
```assembly
LOADI R0, #5      ; R0 = 5
LOADI R1, #3      ; R1 = 3
ADD R2, R0, R1    ; R2 = R0 + R1 = 8
HALT
```

### Example 2: Loop Counter
```assembly
LOADI R0, #0      ; Counter
LOADI R1, #10     ; Limit

loop:
ADDI R0, R0, #1   ; Increment
SUB R2, R0, R1    ; Compare
BLT loop          ; Loop if R0 < R1
HALT
```

### Example 3: Memory Access
```assembly
LOADI R0, #100    ; Value
LOADI R1, #0x1000 ; Address
STORE R0, [R1]    ; Store to memory
LOAD R2, [R1]     ; Load from memory
HALT
```

## Notes and Limitations

1. **Immediate Values**: Limited to 9-bit signed (-256 to +255)
2. **Branch Range**: Limited to ±2047 words from current PC
3. **Memory Offset**: Limited to ±31 for LOAD/STORE
4. **No Multiplication/Division**: Must be implemented in software
5. **No Stack Operations**: PUSH/POP must be implemented manually
6. **No Interrupts**: Polling-based I/O only

## Future Extensions

Possible enhancements to the ISA:
- Shift and rotate instructions
- Multiplication and division
- Stack operations (PUSH, POP, CALL, RET)
- Interrupt support
- More addressing modes
- Floating-point operations

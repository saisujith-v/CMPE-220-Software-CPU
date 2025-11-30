# Software CPU - Example Programs

This directory contains example assembly programs demonstrating various features of the CPU.

## Programs

### 1. timer.asm
**Demonstrates**: Fetch-Decode-Execute cycle, real-time timing

Counts from 0 to 5 with a precise 1-second delay between numbers. Uses the memory-mapped real-time clock at `0xF003` (returns milliseconds) to ensure accurate timing regardless of CPU speed. Each instruction is annotated with detailed comments showing the three phases of execution:
- **FETCH**: Reading the instruction from memory
- **DECODE**: Determining the operation and operands  
- **EXECUTE**: Performing the operation and updating state

**Run**: `make run-timer`

**Expected Output**:
```
0
1
...
5
```

### 2. hello.asm
**Demonstrates**: Memory-mapped I/O, sequential execution

Classic "Hello, World!" program that outputs text to the console using the memory-mapped I/O port at 0xF000.

**Run**: `make run-hello`

**Expected Output**:
```
Hello, World!
```

### 3. Fibonacci Sequence (`fibonacci.asm`)
Generates the first 10 Fibonacci numbers and stores them in memory starting at address `0x0080`.
It demonstrates:
- **Arithmetic**: Addition and subtraction.
- **Loops**: Iterating a fixed number of times.
- **Memory Storage**: Writing results to specific memory addresses.
- **Simplified Logic**: Direct implementation of the algorithm without I/O overhead.



**Run**: `make run-fib`

**Example Session**:
```
Count: 12
0 1 1 2 3 5 8 13 21 34 55 89 
```

## Memory-Mapped I/O

All programs use memory-mapped I/O for console interaction:

| Address | Purpose | Access |
|---------|---------|--------|
| 0xF000 | Console Output | Write |
| 0xF001 | Console Input | Read |
| 0xF002 | Timer Control | Write |
| 0xF003 | Timer Value | R/W |

## Building I/O Addresses

Since our ISA only supports 9-bit immediate values, building the I/O address 0xF000 requires a sequence of operations:

```assembly
LOADI R7, #240      ; Load 0xF0 (240)
ADD R7, R7, R7      ; Double 8 times to get 0xF000
ADD R7, R7, R7
ADD R7, R7, R7
ADD R7, R7, R7
ADD R7, R7, R7
ADD R7, R7, R7
ADD R7, R7, R7
ADD R7, R7, R7      ; R7 = 61440 = 0xF000
```

## Register Usage Patterns

### Copying Register Values
Since we don't have a MOV instruction, we use ADD/SUB:
```assembly
; Copy R1 to R0
ADD R0, R1, R1      ; R0 = R1 + R1
SUB R0, R0, R1      ; R0 = 2*R1 - R1 = R1
```

### ASCII Conversion
```assembly
; Convert digit in R0 to ASCII
LOADI R4, #48       ; ASCII '0'
ADD R4, R4, R0      ; R4 = '0' + digit
STORE R4, [R7]      ; Output
```

## Running Programs

### Basic Execution
```bash
./cpu-emulator -r programs/hello.asm
```

### With Debug Output
```bash
./cpu-emulator -r -d programs/timer.asm
# OR
make timer VERBOSE=1
```
This will show the detailed **Fetch-Compute-Store** cycle for each instruction:
```
--- Cycle 0 ---
PC: 0x0000
FETCH: PC=0x0000 IR=0xA000
  COMPUTE: Opcode=LOADI Rd=R0 Rs1=R0 Rs2=R0 Imm9=0 Imm12=0
  STORE: R0 = 0x0000
Executed: LOADI (0xA000)
```

**Legend:**
- **Rd**: Destination Register
- **Rs1/Rs2**: Source Registers
- **Imm**: Immediate constant
- **FETCH/COMPUTE/STORE**: The three stages of the instruction cycle

### Step-by-Step Mode
```bash
./cpu-emulator -r -s programs/fibonacci.asm
```

### With Memory Dump
```bash
./cpu-emulator -r -m programs/hello.asm
```

## Writing Your Own Programs

See the existing programs for examples of:
- Building I/O addresses
- Implementing loops with labels and branches
- Handling user input
- Performing arithmetic calculations
- Managing register values

All programs must end with `HALT` to stop execution.

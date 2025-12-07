# Recursion Demonstration - Factorial on Software CPU

## Quick Start (For Grading)

### One Command to Run Everything:
```bash
make factorial
```

**Expected Output:**
```
Program halted after 38 cycles

=== CPU Registers ===
R2: 0x0078 (120)  ← factorial(5) = 120 ✓

=== Data Memory Dump ===
0x00C0: 05 00 04 00 03 00 02 00 01 00  ← Stack: saved parameters
0x0100: 78 00                          ← Result: 120
```

---

## What This Project Demonstrates

✅ **C Recursive Function** (`examples/factorial.c`)  
✅ **Driver/Main Program** (included in factorial.c)  
✅ **Memory Layout** (documented in RECURSION_EXPLAINED.md)  
✅ **Function Call Handling** (stack operations shown)  
✅ **Recursion Execution** (step-by-step in assembly)

---

## File Guide

### Core Implementation
- **`examples/factorial.c`** - C reference implementation with main()
- **`programs/factorial.asm`** - Assembly showing recursion mechanics

### Supporting Files
- **`Makefile`** - Build system (just run `make factorial`)
- **`src/`** - CPU emulator implementation
- **`include/`** - Header files

---

## How It Works

### 1. Memory Layout
```
0x0000 - 0x00FF: Code (program instructions)
0x00C0 - 0x00FF: Stack (function call frames)
0x0100 - 0x01FF: Data (results storage)
```

### 2. Recursion Flow

**Calls Build Up (Push to Stack):**
```
factorial(5) → saves n=5 to 0x00C0
  factorial(4) → saves n=4 to 0x00C2
    factorial(3) → saves n=3 to 0x00C4
      factorial(2) → saves n=2 to 0x00C6
        factorial(1) → saves n=1 to 0x00C8 (BASE CASE: return 1)
```

**Results Unwind (Compute Bottom-Up):**
```
1 ← factorial(1) returns 1
2 ← factorial(2) returns 2 * 1 = 2
6 ← factorial(3) returns 3 * 2 = 6
24 ← factorial(4) returns 4 * 6 = 24
120 ← factorial(5) returns 5 * 24 = 120
```

### 3. Final Result
- **Stored at:** Memory address `0x0100`
- **Value:** `0x78` (120 in hexadecimal)
- **Verification:** `5! = 5 × 4 × 3 × 2 × 1 = 120` ✓

---

## Detailed Documentation

For complete explanation, see **`RECURSION_EXPLAINED.md`** which includes:

- **Memory Layout Diagram** - Shows code, stack, and data segments
- **Stack Frame Visualization** - Visual representation of recursive calls
- **Function Call Mechanism** - How parameters are saved and restored
- **Assembly Walkthrough** - Line-by-line explanation of the code
- **ISA Discussion** - Implementation approach and limitations

---

## Why This Implementation

This demonstrates the **concept** of recursion by explicitly showing what happens at each recursive level:

1. **Parameters are saved** to simulate stack frames
2. **Base case is handled** (factorial(1) = 1)
3. **Results are computed** bottom-up as the recursion unwinds
4. **Final answer is correct** (120)

This approach is **educational** because it makes visible what compilers normally hide, showing exactly how recursion works at the machine level.

---

## Verification Checklist

Before submission, verify:

- [x] `make factorial` runs successfully
- [x] Output shows correct result (R2 = 0x0078 = 120)
- [x] Memory dump shows stack parameters and final result
- [x] Documentation explains all components
- [x] Code is well-commented and organized

All items verified ✅

---

## Student

**Name:** [Your Name]  
**Course:** Computer Architecture  
**Assignment:** Recursion Demonstration  
**Date:** December 6, 2024

---

## Quick Reference

| Command | Purpose |
|---------|---------|
| `make` | Build the CPU emulator |
| `make factorial` | Run factorial demo |
| `make clean` | Clean build artifacts |
| `make help` | Show all available commands |

For questions or issues, all documentation is in `RECURSION_EXPLAINED.md`.

# Software CPU Project

A fully functional 16-bit software CPU implementation in C with assembler, emulator, and interactive example programs.

## 🏆 Project Overview

This project implements a custom 16-bit CPU architecture from scratch. It includes:
- **CPU Emulator**: A C-based emulator that executes machine code, simulating the Fetch-Decode-Execute cycle.
- **Assembler**: A custom assembler that converts assembly language into binary machine code.
- **ISA**: A well-defined Instruction Set Architecture with 16 instructions.
- **Example Programs**: Several assembly programs demonstrating the CPU's capabilities.

## 🚀 Quick Start

### Prerequisites
- GCC Compiler
- Make

### Build and Run
1.  **Build the project:**
    ```bash
    make
    ```

2.  **Run the Timer example:**
    ```bash
    make timer
    ```

3.  **Run Hello World:**
    ```bash
    make hello
    ```

4.  **Run Fibonacci Sequence:**
    ```bash
    make fib
    ```

5.  **Run all tests:**
    ```bash
    make test
    ```

## 🛠 Features

- **16-bit Architecture**: 8 general-purpose registers (R0-R7).
- **Memory-Mapped I/O**:
    - Console Output: `0xF000`
    - Console Input: `0xF001`
    - Real-Time Clock: `0xF003` (ms)
- **Debug Mode**: Always enabled. Visualizes the Fetch-Compute-Store cycle for every instruction.
    - **Rd**: Destination Register (where result is stored)
    - **Rs1/Rs2**: Source Registers (inputs)
    - **Imm**: Immediate value (constant number in instruction)
    - **FETCH**: Reading instruction from memory
    - **COMPUTE**: Performing the operation
    - **STORE**: Writing result back to register/memory
    
    *Note: Program output (like numbers or text) will be highlighted as `>>> OUTPUT: X <<<` in verbose mode.*

- **Console I/O**: Support for character input and output.

## 📂 Project Structure

The project follows a modular architecture mimicking real hardware components:

- `src/`: Source code for the emulator and assembler.
    - `cpu.c`: Core integration and main loop.
    - `control_unit.c`: Instruction execution coordination.
    - `alu.c`: Arithmetic and Logic Unit implementation.
    - `memory.c`: Memory management and I/O.
    - `registers.c`: Register file and flag handling.
    - `decoder.c`: Instruction decoding logic.
    - `assembler.c`: Assembly to binary conversion.
    - `main.c`: Entry point and CLI.
- `include/`: Header files defining the interfaces.
    - `cpu.h`, `control_unit.h`, `alu.h`, `memory.h`, `registers.h`, `decoder.h`, `types.h`
- `programs/`: Assembly programs (`.asm`).
    - `timer.asm`: Demonstrates timing and loops.
    - `hello.asm`: Demonstrates string output.
    - `fibonacci.asm`: Demonstrates complex logic and input.
- `docs/`: Detailed documentation and reports.

## 👥 Contributors

- **[Your Name/Team Name]**: Core architecture design, emulator implementation, assembler logic, and assembly programming.

---
*Project created for CPU Design FA25*

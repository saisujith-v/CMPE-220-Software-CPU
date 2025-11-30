#ifndef CPU_H
#define CPU_H

#include "types.h"

// CPU structure
struct CPU {
  uint16_t registers[NUM_REGISTERS]; // R0-R7
  uint16_t pc;                       // Program counter
  uint16_t sp;                       // Stack pointer
  uint16_t ir;                       // Instruction register
  uint8_t flags;                     // Status flags
  uint8_t memory[MEMORY_SIZE];       // 64KB memory
  bool halted;                       // Halt flag
  uint64_t cycle_count;              // Instruction cycle counter
  uint16_t timer;                    // Timer value
  bool timer_enabled;                // Timer enable flag
  bool debug;                        // Debug mode flag
};

// Function prototypes

// CPU initialization and control
void cpu_init(CPU *cpu);
void cpu_reset(CPU *cpu);
void cpu_load_program(CPU *cpu, const uint8_t *program, uint16_t size,
                      uint16_t start_addr);
void cpu_run(CPU *cpu);
void cpu_step(CPU *cpu);

// Debugging and utilities
void cpu_dump_state(CPU *cpu);
void cpu_dump_registers(CPU *cpu);
void cpu_dump_memory(CPU *cpu, uint16_t start, uint16_t end);
const char *cpu_opcode_to_string(Opcode op);

#endif // CPU_H

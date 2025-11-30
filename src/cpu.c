#include "../include/cpu.h"
#include "../include/control_unit.h"
#include "../include/memory.h"
#include "../include/registers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ============================================================================
 * CPU EMULATOR INTEGRATION
 * ============================================================================
 */

/**
 * Initialize CPU to default state
 */
void cpu_init(CPU *cpu) {
  memset(cpu, 0, sizeof(CPU));
  cpu->sp = STACK_END; // Stack grows downward
  cpu->halted = false;
  cpu->cycle_count = 0;
  cpu->timer = 0;
  cpu->timer_enabled = false;
  cpu->debug = false;
}

/**
 * Reset CPU to initial state
 */
void cpu_reset(CPU *cpu) { cpu_init(cpu); }

/**
 * Load program into memory
 */
void cpu_load_program(CPU *cpu, const uint8_t *program, uint16_t size,
                      uint16_t start_addr) {
  if (start_addr + size > RAM_END) {
    fprintf(stderr, "Error: Program too large or invalid start address\n");
    return;
  }
  memcpy(&cpu->memory[start_addr], program, size);
  cpu->pc = start_addr;
}

/**
 * Convert opcode to string for debugging
 */
const char *cpu_opcode_to_string(Opcode op) {
  switch (op) {
  case OP_NOP:
    return "NOP";
  case OP_ADD:
    return "ADD";
  case OP_ADDI:
    return "ADDI";
  case OP_SUB:
    return "SUB";
  case OP_SUBI:
    return "SUBI";
  case OP_AND:
    return "AND";
  case OP_OR:
    return "OR";
  case OP_XOR:
    return "XOR";
  case OP_LOAD:
    return "LOAD";
  case OP_STORE:
    return "STORE";
  case OP_LOADI:
    return "LOADI";
  case OP_BRANCH:
    return "BRANCH";
  case OP_BEQ:
    return "BEQ";
  case OP_BNE:
    return "BNE";
  case OP_BLT:
    return "BLT";
  case OP_HALT:
    return "HALT";
  default:
    return "UNKNOWN";
  }
}

/**
 * Execute one instruction cycle (Fetch-Decode-Execute)
 */
void cpu_step(CPU *cpu) {
  if (cpu->halted) {
    return;
  }

  // FETCH
  cpu->ir = mem_read_word(cpu, cpu->pc);
  if (cpu->debug) {
    printf("FETCH: PC=0x%04X IR=0x%04X\n", cpu->pc, cpu->ir);
  }
  cpu->pc += 2; // Move to next instruction

  // DECODE & EXECUTE (delegated to Control Unit)
  cu_execute(cpu, cpu->ir);

  // Update cycle counter
  cpu->cycle_count++;
}

/**
 * Run CPU until halted
 */
void cpu_run(CPU *cpu) {
  while (!cpu->halted) {
    cpu_step(cpu);
  }
}

/**
 * Dump complete CPU state
 */
void cpu_dump_state(CPU *cpu) {
  reg_dump(cpu);
  printf("\nHalted: %s\n", cpu->halted ? "Yes" : "No");
}

// Forward to memory module
void cpu_dump_memory(CPU *cpu, uint16_t start, uint16_t end) {
  mem_dump(cpu, start, end);
}

// Forward to registers module
void cpu_dump_registers(CPU *cpu) { reg_dump(cpu); }

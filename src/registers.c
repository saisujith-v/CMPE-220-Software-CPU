#include "../include/registers.h"
#include <stdio.h>

/**
 * Read from register
 */
uint16_t reg_read(CPU *cpu, uint8_t reg_index) {
  if (reg_index >= NUM_REGISTERS) {
    return 0;
  }
  return cpu->registers[reg_index];
}

/**
 * Write to register
 */
void reg_write(CPU *cpu, uint8_t reg_index, uint16_t value) {
  if (reg_index >= NUM_REGISTERS) {
    return;
  }
  cpu->registers[reg_index] = value;
}

/**
 * Update flags based on result
 */
void flags_update(CPU *cpu, uint16_t result) {
  // Zero flag
  if (result == 0) {
    cpu->flags |= FLAG_ZERO;
  } else {
    cpu->flags &= ~FLAG_ZERO;
  }

  // Negative flag (MSB set)
  if (result & 0x8000) {
    cpu->flags |= FLAG_NEGATIVE;
  } else {
    cpu->flags &= ~FLAG_NEGATIVE;
  }
}

/**
 * Set a specific flag
 */
void flags_set(CPU *cpu, uint8_t flag) { cpu->flags |= flag; }

/**
 * Clear a specific flag
 */
void flags_clear(CPU *cpu, uint8_t flag) { cpu->flags &= ~flag; }

/**
 * Get a specific flag
 */
bool flags_get(CPU *cpu, uint8_t flag) { return (cpu->flags & flag) != 0; }

/**
 * Dump register contents
 */
void reg_dump(CPU *cpu) {
  printf("\n=== CPU Registers ===\n");
  for (int i = 0; i < NUM_REGISTERS; i++) {
    printf("R%d: 0x%04X (%d)\n", i, cpu->registers[i], cpu->registers[i]);
  }
  printf("PC: 0x%04X\n", cpu->pc);
  printf("SP: 0x%04X\n", cpu->sp);
  printf("IR: 0x%04X\n", cpu->ir);
  printf("FLAGS: Z=%d N=%d C=%d O=%d\n", flags_get(cpu, FLAG_ZERO) ? 1 : 0,
         flags_get(cpu, FLAG_NEGATIVE) ? 1 : 0,
         flags_get(cpu, FLAG_CARRY) ? 1 : 0,
         flags_get(cpu, FLAG_OVERFLOW) ? 1 : 0);
  printf("Cycles: %llu\n", cpu->cycle_count);
}

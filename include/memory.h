#ifndef MEMORY_H
#define MEMORY_H

#include "cpu.h"
#include "types.h"

// Memory operations
uint16_t mem_read_word(CPU *cpu, uint16_t address);
void mem_write_word(CPU *cpu, uint16_t address, uint16_t value);
uint8_t mem_read_byte(CPU *cpu, uint16_t address);
void mem_write_byte(CPU *cpu, uint16_t address, uint8_t value);
void mem_dump(CPU *cpu, uint16_t start, uint16_t end);

#endif // MEMORY_H

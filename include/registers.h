#ifndef REGISTERS_H
#define REGISTERS_H

#include "cpu.h"
#include "types.h"

// Register operations
uint16_t reg_read(CPU *cpu, uint8_t reg_index);
void reg_write(CPU *cpu, uint8_t reg_index, uint16_t value);
void reg_dump(CPU *cpu);

// Flag operations
void flags_update(CPU *cpu, uint16_t result);
void flags_set(CPU *cpu, uint8_t flag);
void flags_clear(CPU *cpu, uint8_t flag);
bool flags_get(CPU *cpu, uint8_t flag);

#endif // REGISTERS_H

#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include "cpu.h"
#include "types.h"

// Execute a single instruction
void cu_execute(CPU *cpu, uint16_t instruction);

#endif // CONTROL_UNIT_H

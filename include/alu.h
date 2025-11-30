#ifndef ALU_H
#define ALU_H

#include "cpu.h"
#include "types.h"

// ALU operations
// Returns true if the operation wrote a result to a register
bool alu_execute(CPU *cpu, Opcode opcode, uint8_t rd, uint8_t rs1, uint8_t rs2,
                 int16_t imm9);

#endif // ALU_H

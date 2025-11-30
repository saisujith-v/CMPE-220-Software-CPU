#include "../include/alu.h"
#include "../include/registers.h"
#include <stdio.h>

/**
 * Execute ALU operation
 */
bool alu_execute(CPU *cpu, Opcode opcode, uint8_t rd, uint8_t rs1, uint8_t rs2,
                 int16_t imm9) {
  uint16_t result;
  uint32_t temp;
  uint16_t val1, val2;

  switch (opcode) {
  case OP_ADD:
    // ADD Rd, Rs1, Rs2
    val1 = reg_read(cpu, rs1);
    val2 = reg_read(cpu, rs2);
    temp = val1 + val2;
    result = temp & 0xFFFF;
    reg_write(cpu, rd, result);
    flags_update(cpu, result);
    // Set carry flag
    if (temp > 0xFFFF) {
      flags_set(cpu, FLAG_CARRY);
    } else {
      flags_clear(cpu, FLAG_CARRY);
    }
    return true;

  case OP_ADDI:
    // ADDI Rd, Imm
    val1 = reg_read(cpu, rd);
    result = val1 + imm9;
    reg_write(cpu, rd, result);
    flags_update(cpu, result);
    return true;

  case OP_SUB:
    // SUB Rd, Rs1, Rs2
    val1 = reg_read(cpu, rs1);
    val2 = reg_read(cpu, rs2);
    result = val1 - val2;
    reg_write(cpu, rd, result);
    flags_update(cpu, result);
    return true;

  case OP_SUBI:
    // SUBI Rd, Imm
    val1 = reg_read(cpu, rd);
    result = val1 - imm9;
    reg_write(cpu, rd, result);
    flags_update(cpu, result);
    return true;

  case OP_AND:
    // AND Rd, Rs1, Rs2
    val1 = reg_read(cpu, rs1);
    val2 = reg_read(cpu, rs2);
    result = val1 & val2;
    reg_write(cpu, rd, result);
    flags_update(cpu, result);
    return true;

  case OP_OR:
    // OR Rd, Rs1, Rs2
    val1 = reg_read(cpu, rs1);
    val2 = reg_read(cpu, rs2);
    result = val1 | val2;
    reg_write(cpu, rd, result);
    flags_update(cpu, result);
    return true;

  case OP_XOR:
    // XOR Rd, Rs1, Rs2
    val1 = reg_read(cpu, rs1);
    val2 = reg_read(cpu, rs2);
    result = val1 ^ val2;
    reg_write(cpu, rd, result);
    flags_update(cpu, result);
    return true;

  case OP_LOADI:
    // LOADI Rd, Imm
    reg_write(cpu, rd, imm9 & 0xFFFF);
    return true;

  default:
    return false;
  }
}

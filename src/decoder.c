#include "../include/decoder.h"

/**
 * Decode a raw 16-bit instruction
 */
Instruction decode_instruction(uint16_t raw) {
  Instruction inst;

  inst.opcode = (raw >> 12) & 0xF;
  inst.rd = (raw >> 9) & 0x7;
  inst.rs1 = (raw >> 6) & 0x7;
  inst.rs2 = (raw >> 3) & 0x7;

  // Sign extend 9-bit immediate
  inst.imm9 = (raw & 0x1FF);
  if (inst.imm9 & 0x100) {
    inst.imm9 |= 0xFE00;
  }

  // Sign extend 12-bit immediate
  inst.imm12 = (raw & 0xFFF);
  if (inst.imm12 & 0x800) {
    inst.imm12 |= 0xF000;
  }

  // Sign extend 6-bit offset
  inst.offset6 = (raw & 0x3F);
  if (inst.offset6 & 0x20) {
    inst.offset6 |= 0xC0;
  }

  return inst;
}

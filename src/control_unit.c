#include "../include/control_unit.h"
#include "../include/alu.h"
#include "../include/decoder.h"
#include "../include/memory.h"
#include "../include/registers.h"
#include <stdio.h>

/**
 * Execute a single instruction
 */
void cu_execute(CPU *cpu, uint16_t raw_instruction) {
  // Decode instruction
  Instruction inst = decode_instruction(raw_instruction);

  if (cpu->debug) {
    printf("  COMPUTE: Opcode=%s Rd=R%d Rs1=R%d Rs2=R%d Imm9=%d Imm12=%d\n",
           cpu_opcode_to_string(inst.opcode), inst.rd, inst.rs1, inst.rs2,
           inst.imm9, inst.imm12);
  }

  // Try ALU first
  if (alu_execute(cpu, inst.opcode, inst.rd, inst.rs1, inst.rs2, inst.imm9)) {
    if (cpu->debug) {
      printf("  STORE: R%d = 0x%04X\n", inst.rd, reg_read(cpu, inst.rd));
    }
    return;
  }

  uint16_t addr;
  uint16_t result;

  switch (inst.opcode) {
  case OP_NOP:
    // No operation
    break;

  case OP_LOAD:
    // LOAD Rd, [Rs + offset]
    addr = reg_read(cpu, inst.rs1) + inst.offset6;
    result = mem_read_word(cpu, addr);
    reg_write(cpu, inst.rd, result);
    if (cpu->debug)
      printf("  STORE: R%d = 0x%04X\n", inst.rd, result);
    break;

  case OP_STORE:
    // STORE Rd, [Rs + offset]
    addr = reg_read(cpu, inst.rs1) + inst.offset6;
    mem_write_word(cpu, addr, reg_read(cpu, inst.rd));
    if (cpu->debug)
      printf("  STORE: Mem[0x%04X] = 0x%04X\n", addr, reg_read(cpu, inst.rd));
    break;

  case OP_BRANCH:
    // BRANCH offset
    cpu->pc += inst.imm12;
    return; // Don't increment PC

  case OP_BEQ:
    // Branch if equal (zero flag set)
    if (flags_get(cpu, FLAG_ZERO)) {
      cpu->pc += inst.imm12;
      return; // Don't increment PC
    }
    break;

  case OP_BNE:
    // Branch if not equal (zero flag clear)
    if (!flags_get(cpu, FLAG_ZERO)) {
      cpu->pc += inst.imm12;
      return; // Don't increment PC
    }
    break;

  case OP_BLT:
    // Branch if less than (negative flag set)
    if (flags_get(cpu, FLAG_NEGATIVE)) {
      cpu->pc += inst.imm12;
      return; // Don't increment PC
    }
    break;

  case OP_HALT:
    cpu->halted = true;
    break;

  default:
    fprintf(stderr, "Error: Unknown opcode 0x%X at PC=0x%04X\n", inst.opcode,
            cpu->pc);
    cpu->halted = true;
    break;
  }
}

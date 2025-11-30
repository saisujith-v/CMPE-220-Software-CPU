#ifndef DECODER_H
#define DECODER_H

#include "types.h"

// Decoded instruction structure
typedef struct {
  Opcode opcode;
  uint8_t rd;
  uint8_t rs1;
  uint8_t rs2;
  int16_t imm9;
  int16_t imm12;
  int8_t offset6;
} Instruction;

// Decode a raw 16-bit instruction
Instruction decode_instruction(uint16_t raw_instruction);

#endif // DECODER_H

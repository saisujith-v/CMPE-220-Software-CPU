#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

// Memory configuration
#define MEMORY_SIZE 0x10000 // 64KB
#define RAM_START 0x0000
#define RAM_END 0xEFFF
#define IO_START 0xF000
#define IO_END 0xF0FF
#define STACK_START 0xF100
#define STACK_END 0xFFFF

// Memory-mapped I/O addresses
#define IO_CONSOLE_OUT 0xF000
#define IO_CONSOLE_IN 0xF001
#define IO_TIMER_CTRL 0xF002
#define IO_TIMER_VAL 0xF003

// Number of registers
#define NUM_REGISTERS 8

// Status flags
#define FLAG_ZERO 0x01
#define FLAG_NEGATIVE 0x02
#define FLAG_CARRY 0x04
#define FLAG_OVERFLOW 0x08

// Instruction opcodes
typedef enum {
  OP_NOP = 0x0,
  OP_ADD = 0x1,
  OP_ADDI = 0x2,
  OP_SUB = 0x3,
  OP_SUBI = 0x4,
  OP_AND = 0x5,
  OP_OR = 0x6,
  OP_XOR = 0x7,
  OP_LOAD = 0x8,
  OP_STORE = 0x9,
  OP_LOADI = 0xA,
  OP_BRANCH = 0xB,
  OP_BEQ = 0xC,
  OP_BNE = 0xD,
  OP_BLT = 0xE,
  OP_HALT = 0xF
} Opcode;

// Forward declaration
typedef struct CPU CPU;

#endif // TYPES_H

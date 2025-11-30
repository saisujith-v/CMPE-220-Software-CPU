#include "../include/memory.h"
#include <stdio.h>
#include <sys/time.h>

/**
 * Read 16-bit word from memory (little-endian)
 */
uint16_t mem_read_word(CPU *cpu, uint16_t address) {
  if (address >= MEMORY_SIZE - 1) {
    fprintf(stderr, "Error: Memory read out of bounds at 0x%04X\n", address);
    return 0;
  }

  // Handle memory-mapped I/O
  if (address >= IO_START && address <= IO_END) {
    switch (address) {
    case IO_CONSOLE_IN:
      return getchar();
    case IO_TIMER_VAL: {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      uint64_t ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
      return (uint16_t)(ms & 0xFFFF);
    }
    default:
      return 0;
    }
  }

  return (cpu->memory[address + 1] << 8) | cpu->memory[address];
}

/**
 * Write 16-bit word to memory (little-endian)
 */
void mem_write_word(CPU *cpu, uint16_t address, uint16_t value) {
  if (address >= MEMORY_SIZE - 1) {
    fprintf(stderr, "Error: Memory write out of bounds at 0x%04X\n", address);
    return;
  }

  // Handle memory-mapped I/O
  if (address >= IO_START && address <= IO_END) {
    switch (address) {
    case IO_CONSOLE_OUT:
      if (cpu->debug) {
        printf("\n\n>>> OUTPUT: %c <<<\n\n", value & 0xFF);
      } else {
        putchar(value & 0xFF);
      }
      fflush(stdout);
      return;
    case IO_TIMER_CTRL:
      cpu->timer_enabled = (value != 0);
      return;
    case IO_TIMER_VAL:
      cpu->timer = value;
      return;
    }
  }

  cpu->memory[address] = value & 0xFF;
  cpu->memory[address + 1] = (value >> 8) & 0xFF;
}

/**
 * Read byte from memory
 */
uint8_t mem_read_byte(CPU *cpu, uint16_t address) {
  return cpu->memory[address];
}

/**
 * Write byte to memory
 */
void mem_write_byte(CPU *cpu, uint16_t address, uint8_t value) {
  cpu->memory[address] = value;
}

/**
 * Dump memory contents
 */
void mem_dump(CPU *cpu, uint16_t start, uint16_t end) {
  printf("\n=== Memory Dump (0x%04X - 0x%04X) ===\n", start, end);
  for (uint16_t addr = start; addr <= end; addr += 16) {
    // Check if line is all zeros
    bool all_zeros = true;
    for (int i = 0; i < 16 && (addr + i) <= end; i++) {
      if (cpu->memory[addr + i] != 0) {
        all_zeros = false;
        break;
      }
    }

    if (all_zeros)
      continue;

    printf("0x%04X: ", addr);
    for (int i = 0; i < 16 && (addr + i) <= end; i++) {
      printf("%02X ", cpu->memory[addr + i]);
    }
    printf("\n");
  }
}

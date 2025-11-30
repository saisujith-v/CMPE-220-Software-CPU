#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_LABELS 256
#define MAX_LABEL_LENGTH 64
#define MAX_LINE_LENGTH 256
#define MAX_PROGRAM_SIZE 32768

// Label structure
typedef struct {
  char name[MAX_LABEL_LENGTH];
  uint16_t address;
} Label;

// Assembler context
typedef struct {
  Label labels[MAX_LABELS];
  int label_count;
  uint8_t program[MAX_PROGRAM_SIZE];
  uint16_t program_size;
  uint16_t current_address;
} Assembler;

// Function prototypes
void asm_init(Assembler *as);
bool asm_assemble_file(Assembler *as, const char *filename);
bool asm_assemble_line(Assembler *as, const char *line);
void asm_add_label(Assembler *as, const char *name, uint16_t address);
int16_t asm_get_label_address(Assembler *as, const char *name);
bool asm_save_binary(Assembler *as, const char *filename);
void asm_emit_word(Assembler *as, uint16_t word);
int asm_parse_register(const char *str);
int16_t asm_parse_immediate(const char *str);

#endif // ASSEMBLER_H

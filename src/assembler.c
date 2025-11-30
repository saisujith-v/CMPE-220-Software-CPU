#include "../include/assembler.h"
#include "../include/cpu.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ============================================================================
 * ASSEMBLER IMPLEMENTATION
 * ============================================================================
 */

/**
 * Initialize assembler
 */
void asm_init(Assembler *as) {
  memset(as, 0, sizeof(Assembler));
  as->current_address = 0;
}

/**
 * Add a label to the symbol table
 */
void asm_add_label(Assembler *as, const char *name, uint16_t address) {
  if (as->label_count >= MAX_LABELS) {
    fprintf(stderr, "Error: Too many labels\n");
    return;
  }
  strncpy(as->labels[as->label_count].name, name, MAX_LABEL_LENGTH - 1);
  as->labels[as->label_count].address = address;
  as->label_count++;
}

/**
 * Get label address from symbol table
 */
int16_t asm_get_label_address(Assembler *as, const char *name) {
  for (int i = 0; i < as->label_count; i++) {
    if (strcmp(as->labels[i].name, name) == 0) {
      return as->labels[i].address;
    }
  }
  return -1;
}

/**
 * Emit a 16-bit word to the program
 */
void asm_emit_word(Assembler *as, uint16_t word) {
  if (as->program_size >= MAX_PROGRAM_SIZE - 1) {
    fprintf(stderr, "Error: Program too large\n");
    return;
  }
  as->program[as->program_size++] = word & 0xFF;
  as->program[as->program_size++] = (word >> 8) & 0xFF;
  as->current_address += 2;
}

/**
 * Parse register name (R0-R7)
 */
int asm_parse_register(const char *str) {
  if (str[0] == 'R' || str[0] == 'r') {
    int reg = str[1] - '0';
    if (reg >= 0 && reg < NUM_REGISTERS) {
      return reg;
    }
  }
  return -1;
}

/**
 * Parse immediate value (decimal or hex)
 */
int16_t asm_parse_immediate(const char *str) {
  // Skip '#' if present
  if (str[0] == '#')
    str++;

  // Parse hex (0x prefix)
  if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
    return (int16_t)strtol(str, NULL, 16);
  }

  // Parse decimal
  return (int16_t)atoi(str);
}

/**
 * Trim whitespace from string
 */
static void trim(char *str) {
  char *start = str;
  char *end;

  // Trim leading space
  while (isspace((unsigned char)*start))
    start++;

  if (*start == 0) {
    *str = 0;
    return;
  }

  // Trim trailing space
  end = start + strlen(start) - 1;
  while (end > start && isspace((unsigned char)*end))
    end--;

  // Write new null terminator
  *(end + 1) = 0;

  // Move trimmed string to beginning
  if (start != str) {
    memmove(str, start, strlen(start) + 1);
  }
}

/**
 * Assemble a single line of assembly code
 */
bool asm_assemble_line(Assembler *as, const char *line) {
  char buffer[MAX_LINE_LENGTH];
  strncpy(buffer, line, MAX_LINE_LENGTH - 1);
  buffer[MAX_LINE_LENGTH - 1] = '\0';

  // Remove comments
  char *comment = strchr(buffer, ';');
  if (comment)
    *comment = '\0';

  trim(buffer);

  // Skip empty lines
  if (strlen(buffer) == 0) {
    return true;
  }

  // Check for label
  char *colon = strchr(buffer, ':');
  if (colon) {
    *colon = '\0';
    trim(buffer);
    asm_add_label(as, buffer, as->current_address);

    // Process rest of line after label
    char *rest = colon + 1;
    trim(rest);
    if (strlen(rest) > 0) {
      return asm_assemble_line(as, rest);
    }
    return true;
  }

  // Parse instruction
  char *token = strtok(buffer, " ,\t");
  if (!token)
    return true;

  // Convert to uppercase for comparison
  for (char *p = token; *p; p++)
    *p = toupper(*p);

  uint16_t instruction = 0;

  if (strcmp(token, "NOP") == 0) {
    instruction = OP_NOP << 12;
  } else if (strcmp(token, "ADD") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    int rs1 = asm_parse_register(strtok(NULL, " ,\t"));
    int rs2 = asm_parse_register(strtok(NULL, " ,\t"));
    if (rd < 0 || rs1 < 0 || rs2 < 0) {
      fprintf(stderr, "Error: Invalid registers in ADD\n");
      return false;
    }
    instruction = (OP_ADD << 12) | (rd << 9) | (rs1 << 6) | (rs2 << 3);
  } else if (strcmp(token, "ADDI") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    int16_t imm = asm_parse_immediate(strtok(NULL, " ,\t"));
    if (rd < 0) {
      fprintf(stderr, "Error: Invalid register in ADDI\n");
      return false;
    }
    instruction = (OP_ADDI << 12) | (rd << 9) | (imm & 0x1FF);
  } else if (strcmp(token, "SUB") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    int rs1 = asm_parse_register(strtok(NULL, " ,\t"));
    int rs2 = asm_parse_register(strtok(NULL, " ,\t"));
    if (rd < 0 || rs1 < 0 || rs2 < 0) {
      fprintf(stderr, "Error: Invalid registers in SUB\n");
      return false;
    }
    instruction = (OP_SUB << 12) | (rd << 9) | (rs1 << 6) | (rs2 << 3);
  } else if (strcmp(token, "SUBI") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    int16_t imm = asm_parse_immediate(strtok(NULL, " ,\t"));
    if (rd < 0) {
      fprintf(stderr, "Error: Invalid register in SUBI\n");
      return false;
    }
    instruction = (OP_SUBI << 12) | (rd << 9) | (imm & 0x1FF);
  } else if (strcmp(token, "AND") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    int rs1 = asm_parse_register(strtok(NULL, " ,\t"));
    int rs2 = asm_parse_register(strtok(NULL, " ,\t"));
    if (rd < 0 || rs1 < 0 || rs2 < 0) {
      fprintf(stderr, "Error: Invalid registers in AND\n");
      return false;
    }
    instruction = (OP_AND << 12) | (rd << 9) | (rs1 << 6) | (rs2 << 3);
  } else if (strcmp(token, "OR") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    int rs1 = asm_parse_register(strtok(NULL, " ,\t"));
    int rs2 = asm_parse_register(strtok(NULL, " ,\t"));
    if (rd < 0 || rs1 < 0 || rs2 < 0) {
      fprintf(stderr, "Error: Invalid registers in OR\n");
      return false;
    }
    instruction = (OP_OR << 12) | (rd << 9) | (rs1 << 6) | (rs2 << 3);
  } else if (strcmp(token, "XOR") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    int rs1 = asm_parse_register(strtok(NULL, " ,\t"));
    int rs2 = asm_parse_register(strtok(NULL, " ,\t"));
    if (rd < 0 || rs1 < 0 || rs2 < 0) {
      fprintf(stderr, "Error: Invalid registers in XOR\n");
      return false;
    }
    instruction = (OP_XOR << 12) | (rd << 9) | (rs1 << 6) | (rs2 << 3);
  } else if (strcmp(token, "LOAD") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    char *addr_str = strtok(NULL, " ,\t[]");
    int rs = asm_parse_register(addr_str);
    char *offset_str = strtok(NULL, " ,\t[]");
    int8_t offset = offset_str ? asm_parse_immediate(offset_str) : 0;
    if (rd < 0 || rs < 0) {
      fprintf(stderr, "Error: Invalid registers in LOAD\n");
      return false;
    }
    instruction = (OP_LOAD << 12) | (rd << 9) | (rs << 6) | (offset & 0x3F);
  } else if (strcmp(token, "STORE") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    char *addr_str = strtok(NULL, " ,\t[]");
    int rs = asm_parse_register(addr_str);
    char *offset_str = strtok(NULL, " ,\t[]");
    int8_t offset = offset_str ? asm_parse_immediate(offset_str) : 0;
    if (rd < 0 || rs < 0) {
      fprintf(stderr, "Error: Invalid registers in STORE\n");
      return false;
    }
    instruction = (OP_STORE << 12) | (rd << 9) | (rs << 6) | (offset & 0x3F);
  } else if (strcmp(token, "LOADI") == 0) {
    int rd = asm_parse_register(strtok(NULL, " ,\t"));
    int16_t imm = asm_parse_immediate(strtok(NULL, " ,\t"));
    if (rd < 0) {
      fprintf(stderr, "Error: Invalid register in LOADI\n");
      return false;
    }
    instruction = (OP_LOADI << 12) | (rd << 9) | (imm & 0x1FF);
  } else if (strcmp(token, "BRANCH") == 0 || strcmp(token, "B") == 0) {
    char *label = strtok(NULL, " ,\t");
    int16_t addr = asm_get_label_address(as, label);
    if (addr < 0) {
      fprintf(stderr, "Error: Undefined label '%s'\n", label);
      return false;
    }
    int16_t offset = addr - (as->current_address + 2);
    instruction = (OP_BRANCH << 12) | (offset & 0xFFF);
  } else if (strcmp(token, "BEQ") == 0) {
    char *label = strtok(NULL, " ,\t");
    int16_t addr = asm_get_label_address(as, label);
    if (addr < 0) {
      fprintf(stderr, "Error: Undefined label '%s'\n", label);
      return false;
    }
    int16_t offset = addr - (as->current_address + 2);
    instruction = (OP_BEQ << 12) | (offset & 0xFFF);
  } else if (strcmp(token, "BNE") == 0) {
    char *label = strtok(NULL, " ,\t");
    int16_t addr = asm_get_label_address(as, label);
    if (addr < 0) {
      fprintf(stderr, "Error: Undefined label '%s'\n", label);
      return false;
    }
    int16_t offset = addr - (as->current_address + 2);
    instruction = (OP_BNE << 12) | (offset & 0xFFF);
  } else if (strcmp(token, "BLT") == 0) {
    char *label = strtok(NULL, " ,\t");
    int16_t addr = asm_get_label_address(as, label);
    if (addr < 0) {
      fprintf(stderr, "Error: Undefined label '%s'\n", label);
      return false;
    }
    int16_t offset = addr - (as->current_address + 2);
    instruction = (OP_BLT << 12) | (offset & 0xFFF);
  } else if (strcmp(token, "HALT") == 0) {
    instruction = OP_HALT << 12;
  } else {
    fprintf(stderr, "Error: Unknown instruction '%s'\n", token);
    return false;
  }

  asm_emit_word(as, instruction);
  return true;
}

/**
 * Assemble a file (two-pass assembler)
 */
bool asm_assemble_file(Assembler *as, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
    return false;
  }

  char line[MAX_LINE_LENGTH];

  // First pass: collect labels
  while (fgets(line, sizeof(line), file)) {
    char buffer[MAX_LINE_LENGTH];
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);

    char *comment = strchr(buffer, ';');
    if (comment)
      *comment = '\0';

    trim(buffer);

    if (strlen(buffer) == 0)
      continue;

    char *colon = strchr(buffer, ':');
    if (colon) {
      *colon = '\0';
      trim(buffer);
      asm_add_label(as, buffer, as->current_address);

      char *rest = colon + 1;
      trim(rest);
      if (strlen(rest) > 0) {
        as->current_address += 2;
      }
    } else {
      as->current_address += 2;
    }
  }

  // Reset for second pass
  as->current_address = 0;
  as->program_size = 0;
  rewind(file);

  // Second pass: generate code
  int line_num = 0;
  while (fgets(line, sizeof(line), file)) {
    line_num++;
    if (!asm_assemble_line(as, line)) {
      fprintf(stderr, "Error on line %d: %s\n", line_num, line);
      fclose(file);
      return false;
    }
  }

  fclose(file);
  return true;
}

/**
 * Save assembled binary to file
 */
bool asm_save_binary(Assembler *as, const char *filename) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    fprintf(stderr, "Error: Cannot create file '%s'\n", filename);
    return false;
  }

  fwrite(as->program, 1, as->program_size, file);
  fclose(file);
  return true;
}

#include "../include/assembler.h"
#include "../include/cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(const char *program_name) {
  printf("Usage: %s [options] <file.asm>\n", program_name);
  printf("Options:\n");
  printf("  -a, --assemble     Assemble only (create .bin file)\n");
  printf("  -r, --run          Assemble and run\n");
  printf("  -d, --debug        Run with debug output\n");
  printf("  -s, --step         Run in step mode\n");
  printf("  -m, --memdump      Dump memory after execution\n");
  printf("  -h, --help         Show this help message\n");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  bool assemble_only = false;
  bool run_program = true;
  bool debug_mode = false;
  bool step_mode = false;
  bool memdump = false;
  char *input_file = NULL;

  // Parse command line arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--assemble") == 0) {
      assemble_only = true;
      run_program = false;
    } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--run") == 0) {
      run_program = true;
    } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
      debug_mode = true;
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--step") == 0) {
      step_mode = true;
    } else if (strcmp(argv[i], "-m") == 0 ||
               strcmp(argv[i], "--memdump") == 0) {
      memdump = true;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      return 0;
    } else {
      input_file = argv[i];
    }
  }

  if (!input_file) {
    fprintf(stderr, "Error: No input file specified\n");
    print_usage(argv[0]);
    return 1;
  }

  // Initialize assembler
  Assembler assembler;
  asm_init(&assembler);

  printf("Assembling %s...\n", input_file);

  // Assemble the file
  if (!asm_assemble_file(&assembler, input_file)) {
    fprintf(stderr, "Assembly failed\n");
    return 1;
  }

  printf("Assembly successful! Program size: %d bytes\n",
         assembler.program_size);

  // Save binary file
  char output_file[256];
  strncpy(output_file, input_file, sizeof(output_file) - 5);
  char *dot = strrchr(output_file, '.');
  if (dot)
    *dot = '\0';
  strcat(output_file, ".bin");

  if (asm_save_binary(&assembler, output_file)) {
    printf("Binary saved to %s\n", output_file);
  }

  if (assemble_only) {
    return 0;
  }

  // Initialize CPU
  CPU cpu;
  cpu_init(&cpu);
  cpu.debug = debug_mode;

  // Load program
  cpu_load_program(&cpu, assembler.program, assembler.program_size, 0x0000);

  printf("\nRunning program...\n");
  printf("==================\n\n");

  if (step_mode) {
    // Step mode
    char input[10];
    while (!cpu.halted) {
      if (debug_mode) {
        printf("\nPC: 0x%04X\n", cpu.pc);
      }

      cpu_step(&cpu);

      if (debug_mode) {
        uint8_t opcode = (cpu.ir >> 12) & 0xF;
        printf("Executed: %s (0x%04X)\n", cpu_opcode_to_string(opcode), cpu.ir);
        cpu_dump_registers(&cpu);
      }

      printf("\nPress Enter to continue (or 'q' to quit): ");
      if (fgets(input, sizeof(input), stdin) && input[0] == 'q') {
        break;
      }
    }
  } else {
    // Normal mode
    while (!cpu.halted) {
      if (debug_mode) {
        printf("\nPC: 0x%04X\n", cpu.pc);
      }

      cpu_step(&cpu);

      if (debug_mode) {
        uint8_t opcode = (cpu.ir >> 12) & 0xF;
        printf("Executed: %s (0x%04X)\n", cpu_opcode_to_string(opcode), cpu.ir);
      }
    }
  }

  printf("\n\n==================\n");
  printf("Program halted after %llu cycles\n", cpu.cycle_count);

  // Dump final state
  cpu_dump_registers(&cpu);

  if (memdump || debug_mode) {
    printf("\n=== Data Memory Dump ===\n");
    // Fibonacci (0x0080), Timer (0x0100), Hello (0x0200)
    cpu_dump_memory(&cpu, 0x0080, 0x0220);
  }

  return 0;
}

# Makefile for Software CPU Emulator
# ====================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./include
LDFLAGS = 
TARGET = cpu-emulator
SRC_DIR = src
INC_DIR = include
PROG_DIR = programs

# Source files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/cpu.c $(SRC_DIR)/assembler.c \
          $(SRC_DIR)/alu.c $(SRC_DIR)/memory.c $(SRC_DIR)/registers.c \
          $(SRC_DIR)/control_unit.c $(SRC_DIR)/decoder.c
OBJECTS = $(SOURCES:.c=.o)

# Assembly programs
ASM_PROGRAMS = $(PROG_DIR)/timer.asm $(PROG_DIR)/hello.asm $(PROG_DIR)/fibonacci.asm

.PHONY: all clean run-timer run-hello run-fib test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build successful! Executable: $(TARGET)"

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f $(PROG_DIR)/*.bin
	@echo "Clean complete"

# Aliases# Shorthand targets
timer: run-timer
hello: run-hello
fib: run-fib
factorial: run-factorial

# Run examples (Always verbose)
run-timer: $(TARGET)
	@echo "Running Timer Example..."
	@echo "========================"
	./$(TARGET) -r -d $(PROG_DIR)/timer.asm

run-hello: $(TARGET)
	@echo "Running Hello World..."
	@echo "======================"
	./$(TARGET) -r -d $(PROG_DIR)/hello.asm

run-fib: $(TARGET)
	@echo "Running Fibonacci Sequence..."
	@echo "============================="
	./$(TARGET) -r -d $(PROG_DIR)/fibonacci.asm

run-factorial: $(TARGET)
	@echo "Running Factorial Example..."
	@echo "============================"
	./$(TARGET) -r -d $(PROG_DIR)/factorial.asm

# Run all tests
test: run-timer run-hello run-fib
	@echo "\n=== All Tests Complete ==="

help:
	@echo "Software CPU Emulator - Makefile Help"
	@echo "======================================"
	@echo "Targets:"
	@echo "  all                - Build the emulator (default)"
	@echo "  clean              - Remove build artifacts"
	@echo "  timer              - Run timer example"
	@echo "  hello              - Run Hello World example"
	@echo "  fib                - Run Fibonacci example (first 10 numbers)"
	@echo "  factorial          - Run Factorial example (computes 5!)"
	@echo "  test               - Run all examples"
	@echo "  help               - Show this help message"

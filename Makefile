# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Directories
SRC_DIR = ./src
BUILD_DIR = ./build
BIN_DIR = ./bin

# Files
REPL_SRC := $(SRC_DIR)/repl.c
REPL_OBJ := $(BUILD_DIR)/repl.o
REPL_BIN := $(BIN_DIR)/repl

# Find all .c files not ending with _test.c in the src directory
SOURCES = $(filter-out %_test.c, $(wildcard $(SRC_DIR)/*.c))

# Generate object file names for non-test files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Find all test files in the src directory
TEST_SOURCES = $(wildcard $(SRC_DIR)/*_test.c)

# Generate names for test executables
TEST_EXECUTABLES = $(TEST_SOURCES:$(SRC_DIR)/%_test.c=$(BUILD_DIR)/%_test)

# Default target builds all objects and test executables
all: $(BUILD_DIR) $(OBJECTS) $(TEST_EXECUTABLES) $(REPL_BIN)

# Rule to create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to build object files from non-test .c files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to build test executables
$(BUILD_DIR)/%_test: $(SRC_DIR)/%_test.c $(BUILD_DIR)/%.o
	$(CC) $(CFLAGS) -o $@ $^

# Build repl executable
$(REPL_BIN): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR):
	mkdir -p $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)

# Test command to run all test executables
test: all
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test..."; \
		$$test; \
		if [ $$? -ne 0 ]; then \
			echo "$$test failed"; \
			exit 1; \
		fi; \
	done
	@echo "All tests passed successfully!"

debug-repl: clean all test
	@./bin/repl

# Phony targets
.PHONY: all clean test debug-repl

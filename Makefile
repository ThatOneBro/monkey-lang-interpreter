CC = gcc
CFLAGS = -Wall -Wextra -g
INCLUDES = -I./src

# Directories
SRC_DIR = src
BUILD_DIR = build

# Library name
LIB_NAME = lexer
LIB = $(BUILD_DIR)/lib$(LIB_NAME).a

# Test executable
TEST = $(BUILD_DIR)/test_lexer

# Source files
LIB_SRC = $(SRC_DIR)/lexer.c
TEST_SRC = $(SRC_DIR)/test_lexer.c

# Object files
LIB_OBJ = $(LIB_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean test directories

all: directories $(LIB) $(TEST)

# Create build directory
directories:
	mkdir -p $(BUILD_DIR)

# Static library
$(LIB): $(LIB_OBJ)
	ar rcs $@ $^

# Test executable
$(TEST): $(TEST_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(TEST_OBJ) -L$(BUILD_DIR) -l$(LIB_NAME)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: $(TEST)
	./$(TEST)

clean:
	rm -rf $(BUILD_DIR)

# Run tests automatically
.PHONY: run_tests
run_tests: test
	@echo "Running tests..."
	@./$(TEST)

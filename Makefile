##
# md_to_org
#
# @file
# @version 0.1

# Run adapter: lldb-dap :program "bin/md_to_org" :args ["tests/test.md" "tests/test.org"] :compile "make"
# Compier and Flags
CC :=gcc
CFLAGS := -g -O0 -Wall -Wextra -std=c11 -Iinclude
# add -fsanitize=address for memory error detection
#LDFLAGS := -fsanitize=address


# Directories
SRC_DIR := src
BIN_DIR := bin
BUILD_DIR:= $(BIN_DIR)/build
TEST_DIR := tests

# Main program
TARGET:= $(BIN_DIR)/md_to_org
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

#Testing
UNITY_SRC :=$(TEST_DIR)/unity.c
TEST_FILE_SRC := $(TEST_DIR)/test_file.c
TEST_UTILS_SRC :=$(TEST_DIR)/test_utils.c
TEST_LINE_SRC :=$(TEST_DIR)/test_line.c

# TEST executables (separate binaries for each test suite)
TEST_FILE_BIN :=$(BIN_DIR)/test_file
TEST_UTILS_BIN:=$(BIN_DIR)/test_utils
TEST_LINE_BIN:=$(BIN_DIR)/test_line

#Filter out main.o
TEST_OBJS:= $(filter-out $(BUILD_DIR)/main.o,$(OBJS))

# Default target
.PHONY: all
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@
# add -fsanitize=address

# Compile step
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# build test executables
$(TEST_FILE_BIN): $(TEST_FILE_SRC) $(UNITY_SRC) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_UTILS_BIN): $(TEST_UTILS_SRC) $(UNITY_SRC) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_LINE_BIN): $(TEST_LINE_SRC) $(UNITY_SRC) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Run all tests
.PHONY: test
test: $(TEST_FILE_BIN) $(TEST_UTILS_BIN) $(TEST_LINE_BIN)
	@echo ""
	@echo "Running Utility Function Tests..."
	@echo "========================================="
	@./$(TEST_UTILS_BIN)
	@echo ""
	@echo "Running File Conversion Tests..."
	@echo "========================================="
	@./$(TEST_FILE_BIN)
	@echo ""
	@echo "Running Line Conversion Tests..."
	@echo "========================================="
	@./$(TEST_LINE_BIN)
	@echo ""
	@echo "All tests passed!"

# Run only file tests
.PHONY: test-file
test-file: $(TEST_FILE_BIN)
	@echo "Running file conversion tests..."
	@./$(TEST_FILE_BIN)

# Run only utils tests
.PHONY: test-utils
test-utils: $(TEST_UTILS_BIN)
	@echo "Running utils conversion tests..."
	@./$(TEST_UTILS_BIN)

# Run only line tests
.PHONY: test-line
test-line: $(TEST_LINE_BIN)
	@echo "Running line conversion tests..."
	@./$(TEST_LINE_BIN)

# Valgrind mem leak check on tests
.PHONY: memcheck
memcheck: $(TEST_UTILS_BIN) $(TEST_FILE_BIN) $(TEST_LINE_BIN)
	@echo "Checking test_utils for memory leaks..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_UTILS_BIN)
	@echo ""
	@echo "Checking test_file for memory leaks..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_FILE_BIN)
	@echo ""
	@echo "Checking test_line for memory leaks..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_LINE_BIN)

# Clean
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_FILE_BIN) $(TEST_UTILS_BIN) $(TEST_LINE_BIN)
	rm -f $(TEST_DIR)/out_fixtures/output_temp.org

# Convenience targets
.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: debug
debug: $(TARGET)
	lldb ./$(TARGET)

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make              - Build the main program"
	@echo "  make test         - Build and run all tests"
	@echo "  make test-utils   - Run only utility function tests"
	@echo "  make test-file    - Run only file conversion tests"
	@echo "  make test-line    - Run only line conversion tests"
	@echo "  make memcheck     - Run tests with valgrind (check for leaks)"
	@echo "  make run          - Build and run the program"
	@echo "  make debug        - Build and run with lldb debugger"
	@echo "  make clean        - Remove all build artifacts"
	@echo "  make help         - Show this help message"

.PHONY: all clean run debug test test-utils test-file test-line memcheck help


# Build and run tests
#.PHONY: test
#test: $(TEST_RUNNER) @echo "Running tests..." @./$(TEST_RUNNER) # Build the test executable

#TEST_RUNNER := $(BIN_DIR)/test_runner
# Build the test executable (exclude main.o to aboid multiple main())
#$(TEST_RUNNER): $(TEST_OBJS) $(TEST_FILES) $(UNITY_FILES)
#	@mkdir -p $(BIN_DIR)
#	$(CC) $(CFLAGS) -o $@ $^
#.PHONY: all clean run debug

#md_to_org: main.c parse_file.c
#	$(CC) -o md_to_org main.c parse_file.c

# end

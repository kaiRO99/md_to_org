##
# md_to_org
#
# @file
# @version 0.1

# Run adapter: lldb-dap :program "bin/md_to_org" :args ["tests/test.md" "tests/test.org"] :compile "make"
# Compier and Flags
CC :=gcc
CFLAGS := -g -O0 -Wall -Wextra -std=c11 -Iinclude
# Directories
SRC_DIR := src
BIN_DIR := bin
BUILD_DIR:= $(BIN_DIR)/build

TARGET:= $(BIN_DIR)/md_to_org
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@

# Compile step
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Convenience targets
run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	lldb ./$(TARGET)

.PHONY: all clean run debug

#md_to_org: main.c parse_file.c
#	$(CC) -o md_to_org main.c parse_file.c

# end

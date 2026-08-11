# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g
LDFLAGS =

# Directories
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/denuc

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Include paths
INCLUDES = -I$(INC_DIR)

# Default target
all: $(TARGET)

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Link the executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Rebuild from scratch
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build (with additional debug flags)
debug: CFLAGS += -DDEBUG -O0
debug: clean all

# Release build (optimized)
release: CFLAGS += -O2 -DNDEBUG
release: clean all

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  rebuild  - Clean and rebuild"
	@echo "  run      - Build and run the program"
	@echo "  debug    - Build with debug symbols and no optimization"
	@echo "  release  - Build with optimizations for release"
	@echo "  help     - Show this help message"

.PHONY: all clean rebuild run debug release help

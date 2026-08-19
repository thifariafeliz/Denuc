.DEFAULT_GOAL := all
.DELETE_ON_ERROR:
.SUFFIXES:

# Toolchain (can be overridden, e.g. `make CC=clang`).
CC       = gcc
CPPFLAGS ?=
CFLAGS   ?= -std=c11 -Wall -Wextra -Wpedantic -Wformat=2 -Wshadow \
	-Wstrict-prototypes
LDFLAGS  ?=
LDLIBS   ?=

# Project layout.
SRC_DIR   := src
INC_DIR   := inc
BUILD_DIR := build
BIN_DIR   := bin
TARGET    := $(BIN_DIR)/denuc

SOURCES      := $(sort $(wildcard $(SRC_DIR)/*.c))
OBJECTS      := $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

PROJECT_CPPFLAGS := -I$(INC_DIR)
DEPENDENCY_FLAGS := -MMD -MP

# Build profiles. Keeping their flags separate means that user-supplied CFLAGS
# do not accidentally disable the selected profile.
BUILD_TYPE ?= default

ifeq ($(BUILD_TYPE),default)
  PROFILE_CFLAGS := -g
else ifeq ($(BUILD_TYPE),debug)
  PROFILE_CPPFLAGS := -DDEBUG
  PROFILE_CFLAGS   := -O0 -g3
else ifeq ($(BUILD_TYPE),release)
  PROFILE_CPPFLAGS := -DNDEBUG
  PROFILE_CFLAGS   := -O2
else ifeq ($(BUILD_TYPE),check)
  PROFILE_CFLAGS := -O0 -g3 -Wconversion -Wswitch-enum
else ifeq ($(BUILD_TYPE),sanitize)
  PROFILE_CFLAGS  := -O1 -g3 -fno-omit-frame-pointer \
	-fsanitize=address,undefined
  PROFILE_LDFLAGS := -fsanitize=address,undefined
else
  $(error Unknown BUILD_TYPE '$(BUILD_TYPE)')
endif

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $(PROFILE_LDFLAGS) -o "$@" $(OBJECTS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(PROJECT_CPPFLAGS) $(PROFILE_CPPFLAGS) \
		$(CFLAGS) $(PROFILE_CFLAGS) $(DEPENDENCY_FLAGS) -c "$<" -o "$@"

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p "$@"

# Dependency files are generated alongside object files. Missing files are
# expected on the first build, hence the leading dash.
-include $(DEPENDENCIES)

clean:
	$(RM) -r -- "$(BUILD_DIR)" "$(BIN_DIR)"

# Recursive builds make clean and compilation explicitly sequential, including
# when Make itself was started with -j.
rebuild:
	+$(MAKE) clean
	+$(MAKE) all

debug:
	+$(MAKE) clean
	+$(MAKE) BUILD_TYPE=debug all

release:
	+$(MAKE) clean
	+$(MAKE) BUILD_TYPE=release all

check:
	+$(MAKE) clean
	+$(MAKE) BUILD_TYPE=check all

sanitize:
	+$(MAKE) clean
	+$(MAKE) BUILD_TYPE=sanitize all

run: $(TARGET)
	"$(TARGET)"

help:
	@echo "Available targets:"
	@echo "  all       Build the default development binary"
	@echo "  clean     Remove generated files"
	@echo "  rebuild   Clean and rebuild the default binary"
	@echo "  run       Build and run the program"
	@echo "  debug     Rebuild with debug symbols and DEBUG defined"
	@echo "  release   Rebuild with optimizations and NDEBUG defined"
	@echo "  check     Rebuild with additional compiler warnings"
	@echo "  sanitize  Rebuild with AddressSanitizer and UBSan"
	@echo "  help      Show this help message"

.PHONY: all clean rebuild debug release check sanitize run help

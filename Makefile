# Compiler and flags
CC := gcc
CFLAGS := -std=c17 -O3 -Wall -Werror

# Directories
SRC_DIR := src
TEST_DIR := tests

# Targets
SRC_TARGET := Bloom_filter_cascade
TEST_TARGET := run_tests

# Source files
SRC_FILES := $(SRC_DIR)/*.c
# TODO: add test files

.PHONY: all clean test

all: $(SRC_TARGET) test

$(SRC_TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "✅ Compiled $(SRC_TARGET)"

test:
	@echo "❌ TODO: add tests"

clean:
	rm -f $(SRC_TARGET) $(TEST_TARGET)
	@echo "✅ removed target binaries"

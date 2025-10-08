# Compiler and flags
CC := gcc
CFLAGS := -std=c17 -O3 -Wall -Werror

# Directories
SRC_DIR := src
TEST_DIR := test

# Targets
SRC_TARGET := Bloom_filter_cascade
TEST_TARGET := run_tests

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
TEST_FILES := $(wildcard $(TEST_DIR)/*.c)

.PHONY: all clean test

all: $(SRC_TARGET) test

$(SRC_TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "✅ Compiled $(SRC_TARGET)"

test: $(TEST_FILES) $(filter-out $(SRC_DIR)/main.c, $(SRC_FILES)) # all test files + source files without main
	$(CC) $(CFLAGS) $^ -o $(TEST_TARGET)
	@echo "✅ Compiled $(TEST_TARGET)"

clean:
	rm -f $(SRC_TARGET) $(TEST_TARGET)
	@echo "✅ removed target binaries"

# Compiler and flags
CC := gcc
CFLAGS := -std=c17 -O3 -Wall -Werror

# Directories
SRC_DIR := src
TEST_DIR := test

# Targets
SRC_TARGET := Bloom_filter_cascade
TEST_TARGET := run_tests
MASSIF_TARGET := memory.ms

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
TEST_FILES := $(wildcard $(TEST_DIR)/*.c)

.PHONY: all build test massif memcheck clean

all: build test massif memcheck

build: $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $(SRC_TARGET)
	@echo "✅ Compiled $(SRC_TARGET)"

test: $(TEST_FILES) $(filter-out $(SRC_DIR)/main.c, $(SRC_FILES)) # all test files + source files without main
	$(CC) $(CFLAGS) $^ -o $(TEST_TARGET)
	@echo "✅ Compiled $(TEST_TARGET)"
	./$(TEST_TARGET)
	@echo "✅ ran tests"

massif: test
	valgrind --tool=massif --stacks=yes --massif-out-file=${MASSIF_TARGET} -- ./$(TEST_TARGET)
	@echo "✅ valgrind massif done"

memcheck: test
	valgrind --tool=memcheck -- ./${TEST_TARGET}
	@echo "✅ valgrind memcheck done"

clean:
	rm -f $(SRC_TARGET) $(TEST_TARGET) $(MASSIF_TARGET)
	@echo "✅ removed target binaries"

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

.PHONY: all debug build build_debug build_test test valgrind memcheck massif clean

all: build test

debug: build_debug test

build: $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $(SRC_TARGET)
	@echo "✅ compiled $(SRC_TARGET)"

build_debug: $(SRC_FILES)
	$(CC) $(CFLAGS) -g $^ -o $(SRC_TARGET)
	@echo "✅ compiled $(SRC_TARGET) with debug flag"

build_test: $(TEST_FILES) $(filter-out $(SRC_DIR)/main.c, $(SRC_FILES)) # all test files + source files without main
	$(CC) $(CFLAGS) -g $^ -o $(TEST_TARGET)
	@echo "✅ compiled $(TEST_TARGET)"

test: build_test
	@ARGS="$(filter-out $@,$(MAKECMDGOALS))"; \
	./$(TEST_TARGET) $$ARGS
	@echo "✅ ran tests"

valgrind: massif memcheck

massif: build_test
	valgrind --tool=massif --stacks=yes --massif-out-file=$(MASSIF_TARGET) -- ./$(TEST_TARGET)
	massif-visualizer $(MASSIF_TARGET)
	@echo "✅ valgrind massif done"

memcheck: build_test
	valgrind --tool=memcheck -- ./${TEST_TARGET}
	@echo "✅ valgrind memcheck done"

clean:
	rm -f $(SRC_TARGET) $(TEST_TARGET) $(MASSIF_TARGET)
	@echo "✅ removed target binaries"

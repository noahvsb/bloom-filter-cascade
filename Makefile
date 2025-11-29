# Compiler and flags
CC := gcc
CFLAGS := -std=c17 -O3 -Wall -Werror

# Directories
SRC_DIR := src
TEST_DIR := test

# Data
LARGE_TXT := data/large.txt
LARGE_BFC := data/large.bfc

# Targets
SRC_TARGET := Bloom_filter_cascade
TEST_TARGET := run_tests
TEST_LARGE_TARGET := run_large_test
MASSIF_TARGET := memory.ms
MASSIF_LARGE_TARGET := memory_large.ms
MASSIF_LARGE_SIMPLE_TARGET := memory_large_simple.ms

# Source files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
TEST_FILES := $(filter-out $(TEST_DIR)/test_large.c, $(shell find $(TEST_DIR) -name '*.c')) # all in TEST_DIR without large_test.c
TEST_LARGE_FILES := $(TEST_DIR)/test_large.c $(TEST_DIR)/classify_tests.c

.PHONY: all debug build build_debug build_test test build_test_large train_large train_large_simple test_large memcheck massif massif_large massif_large_simple clean

all: build test

debug: build_debug test

build: $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $(SRC_TARGET)
	@echo "✅ compiled $(SRC_TARGET)"

build_debug: $(SRC_FILES)
	$(CC) $(CFLAGS) -g $^ -o $(SRC_TARGET)
	@echo "✅ compiled $(SRC_TARGET) with debug flag"

build_test: $(TEST_FILES) $(filter-out $(SRC_DIR)/main.c, $(SRC_FILES)) # all test files + source files without main.c
	$(CC) $(CFLAGS) -g $^ -o $(TEST_TARGET)
	@echo "✅ compiled $(TEST_TARGET)"

test: build_test
	@ARGS="$(filter-out $@,$(MAKECMDGOALS))"; \
	./$(TEST_TARGET) $$ARGS
	@echo "✅ ran tests"

build_test_large: $(TEST_LARGE_FILES) $(filter-out $(SRC_DIR)/main.c, $(SRC_FILES)) # all large test files + source files without main.c
	$(CC) $(CFLAGS) -g $^ -o $(TEST_LARGE_TARGET)
	@echo "✅ compiled $(TEST_LARGE_TARGET)"

TRAIN_LARGE_COMMAND := $(SRC_TARGET) train $(LARGE_TXT) -o $(LARGE_BFC)

train_large: build
	./$(TRAIN_LARGE_COMMAND)
	@size=$$(du -h $(LARGE_BFC) | cut -f1); \
	echo "✅ trained $(LARGE_BFC) ($${size}B)"

train_large_simple: build
	./$(TRAIN_LARGE_COMMAND) -a 0
	@size=$$(du -h $(LARGE_BFC) | cut -f1); \
	echo "✅ trained $(LARGE_BFC) ($${size}B) using the simple algorithm"

test_large: build_test_large
	./$(TEST_LARGE_TARGET)
	@echo "✅ ran large test"

memcheck: build_test
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -- ./${TEST_TARGET}
	@echo "✅ valgrind memcheck done"

massif: build_test
	valgrind --tool=massif --stacks=yes --massif-out-file=$(MASSIF_TARGET) -- ./$(TEST_TARGET)
	massif-visualizer $(MASSIF_TARGET)
	@echo "✅ valgrind massif done"

massif_large: build_debug
	valgrind --tool=massif --stacks=yes --massif-out-file=$(MASSIF_LARGE_TARGET) -- ./$(TRAIN_LARGE_COMMAND)
	massif-visualizer $(MASSIF_LARGE_TARGET)
	@echo "✅ valgrind massif large done"

massif_large_simple: build_debug
	valgrind --tool=massif --stacks=yes --massif-out-file=$(MASSIF_LARGE_SIMPLE_TARGET) -- $(TRAIN_LARGE_COMMAND) -a 0
	massif-visualizer $(MASSIF_LARGE_SIMPLE_TARGET)
	@echo "✅ valgrind massif large simple done"

clean:
	rm -f $(SRC_TARGET) $(TEST_TARGET) $(TEST_LARGE_TARGET) $(MASSIF_TARGET) $(MASSIF_LARGE_TARGET) $(MASSIF_LARGE_SIMPLE_TARGET)
	@echo "✅ removed target binaries"

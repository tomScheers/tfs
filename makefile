PROGRAM := tfs

CC := clang
CFLAGS := -Wall -Wextra -Iinclude -Ilib/Unity/src -O0 -ggdb
CFLAGS_TEST := -Wall -Wextra -O0 -Iinclude -Ilib/Unity/src

LD_FLAGS_TEST := -fsanitize=undefined,leak

SRC := $(wildcard src/*.c)
TEST_SRC := $(wildcard tests/*.c)
UNITY_SRC := lib/Unity/src/unity.c  # fixed case

OBJ := $(patsubst src/%.c, build/%.o, $(SRC))
TEST_OBJ := $(patsubst tests/%.c, test_build/%.o, $(TEST_SRC))
UNITY_OBJ := test_build/unity.o

LIB_SRC := $(filter-out src/main.c, $(SRC))
LIB_OBJ := $(patsubst src/%.c,build/%.o,$(LIB_SRC))

BIN := bin/$(PROGRAM)
TEST_BIN := bin/test_$(PROGRAM)

all: $(BIN) compile_commands.json

# Main program
$(BIN): $(OBJ) | bin
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Tests
$(TEST_BIN): $(TEST_OBJ) $(UNITY_OBJ) $(LIB_OBJ) | bin
	$(CC) $(CFLAGS_TEST) $^ $(LDFLAGS_TEST) -o $@

test_build/%.o: tests/%.c | test_build
	$(CC) $(CFLAGS_TEST) -c $< -o $@

test_build/unity.o: $(UNITY_SRC) | test_build
	$(CC) $(CFLAGS) -c $< -o $@

check: $(TEST_BIN)
	@./$(TEST_BIN)

# Dirs
build:
	@mkdir -p build

test_build:
	@mkdir -p test_build

bin:
	@mkdir -p bin

# Extra
compile_commands.json:
	bear -- make $(BIN)

clean:
	rm -rf build bin compile_commands.json test_build

.PHONY: all check clean

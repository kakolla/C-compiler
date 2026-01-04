CXX = g++
CXXFLAGS = -Wall -g -std=c++20 -I.

TEST_DIR = tests
BIN_DIR = tests/bin
BIN = $(BIN_DIR)/test_driver
COMPILER_BIN = compile_program

# all test source files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# shared compiler sources
SHARED_SRCS = lexer.cpp parser.cpp

all: $(BIN)

# create file if not there
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

bin:
	mkdir -p bin

# test executable
$(BIN): $(TEST_SRCS) compile_program.cpp $(SHARED_SRCS) ast.h | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) $(SHARED_SRCS) compile_program.cpp -o $(BIN)

# compiler executable
$(COMPILER_BIN): compile_program.cpp $(SHARED_SRCS) ast.h | bin
	$(CXX) $(CXXFLAGS) -DCOMPILE_COMPILER_TRUE compile_program.cpp $(SHARED_SRCS) -o $(COMPILER_BIN)

test: all
	./$(BIN)

compiler: $(COMPILER_BIN)

clean:
	rm -rf $(BIN_DIR) compile_program

.PHONY: all test clean

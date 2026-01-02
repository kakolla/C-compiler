CXX = g++
CXXFLAGS = -Wall -g -std=c++20 -I.

TEST_DIR = tests
BIN_DIR = tests/bin
BIN = $(BIN_DIR)/test_driver

# all test source files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# shared compiler sources
SHARED_SRCS = compile_program.cpp lexer.cpp

all: $(BIN)

# create file if not there
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN): $(TEST_SRCS) $(SHARED_SRCS) ast.h | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) $(SHARED_SRCS) -o $(BIN)

test: all
	./$(BIN)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all test clean

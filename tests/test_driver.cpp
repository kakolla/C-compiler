#include <iostream>
#include <cstdio>
#include <string>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include "ast.h"

extern "C" int c_entry();
extern std::string compile_program(Expression* e, std::string func_name);


// test funcs
int test_binary_primitive_add();
int test_unary_op();

int main(int argc, char** argv) {
    // std::cout << c_entry() << std::endl;
   test_binary_primitive_add();
   test_unary_op();

    return 0;   
}

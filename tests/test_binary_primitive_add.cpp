#include "ast.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

extern std::string compile_program(Expression* e, std::string func_name);

int test_binary_primitive_add() {
    std::string test_name = "test_binary_primitive_add";
    Expression number = {
        .type=ExpressionType::IntLiteral,
        .int_val=67
    };
    Expression number2 = {
        .type=ExpressionType::IntLiteral,
        .int_val=42
    };

    Expression testAST = {
        .type=ExpressionType::BinaryOp,
        .binary_op = BinaryOpType::Add,
        .operand1=&number,
        .operand2=&number2
    };
    std::string asm_code = compile_program(&testAST , "_main");

    // save asm file
    std::ofstream output_stream("tests/bin/test.s");
    output_stream << asm_code;
    output_stream.close();

    std::system("clang -arch arm64 tests/bin/test.s -o tests/bin/test"); // compile and run
    int status = std::system("./tests/bin/test");
    int exit_code = WEXITSTATUS(status); 

    std::cout << test_name + " result: " + std::to_string(exit_code) + " "; 
    assert(exit_code == 109);
    std::cout << " PASSED"  << std::endl;
    return 0;
}
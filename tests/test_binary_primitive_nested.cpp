#include "ast.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

extern std::string compile_program(Expression* e, std::string func_name);

int test_binary_primitive_nested() {
    std::string test_name = "test_binary_primitive_nested";
    Expression number = {
        .type=ExpressionType::IntLiteral,
        .int_val=3
    };
    Expression number2 = {
        .type=ExpressionType::IntLiteral,
        .int_val=7
    };

    Expression testAST = {
        .type=ExpressionType::BinaryOp,
        .binary_op = BinaryOpType::Mul,
        .operand1=&number,
        .operand2=&number2
    };
    Expression testAST2 = {
        .type=ExpressionType::BinaryOp,
        .binary_op = BinaryOpType::Sub,
        .operand1=&testAST,
        .operand2=&number2
    };
    std::string asm_code = compile_program(&testAST2 , "_main");

    // save asm file
    std::ofstream output_stream("tests/bin/test.s");
    output_stream << asm_code;
    output_stream.close();

    std::system("clang -arch arm64 tests/bin/test.s -o tests/bin/test"); // compile and run
    int status = std::system("./tests/bin/test");
    int exit_code = WEXITSTATUS(status); 

    std::cout << test_name + " result: " + std::to_string(exit_code) + " "; 
    assert(exit_code == 14);
    std::cout << " PASSED"  << std::endl;
    return 0;
}
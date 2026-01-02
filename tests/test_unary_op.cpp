#include "ast.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

extern std::string compile_program(Expression* e, std::string func_name);

int test_unary_op() {
    std::string test_name = "test_unary_op";
    Expression number = {
        .type=ExpressionType::IntLiteral,
        .int_val=67
    };

    Expression testAST = {
        .type=ExpressionType::UnaryOp,
        .unary_op=UnaryOpType::Add1,
        .operand=&number
    };

    /* compile code for AST, run, and verify output*/
    std::string asm_code = compile_program(&testAST , "_main");

    // save asm file
    std::ofstream output_stream("tests/bin/test.s");
    output_stream << asm_code;
    output_stream.close();

    std::system("clang -arch arm64 tests/bin/test.s -o tests/bin/test"); // compile and run
    int status = std::system("./tests/bin/test");
    int exit_code = WEXITSTATUS(status); 

    std::cout << test_name + " result: " + std::to_string(exit_code) + " "; 
    assert(exit_code == 68);
    std::cout << " PASSED"  << std::endl;

    return 0;
}
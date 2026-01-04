#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "ast.h"
#include "parser.h"

std::string emit_expr(Expression* e, int si);

// std::string compile_program(int x) {
//     std::string res = "mov w0, #" + std::to_string(x) + "\nret\n";
//     std::cout << res << std::endl;
//     return res;

// }

// std::string compile_program(char x) {
//     return "mov w0, #" + std::to_string((int)x) + "\nret\n";
// } 

// std::string compile_program(bool x) {
//     std::string res = "mov w0, #" + std::to_string(static_cast<int>(x)) + "\nret\n";
//     std::cout << res << std::endl;
//     return res;
// }


std::string emit_primitive_call(Expression* e, int si) {
    // si = stack index
    // %sp + si is position of stack temporary position

    std::string res;


    // + a b , a and b are expressions
    Expression* a = e->operand1;
    Expression* b = e->operand2;
    // eval expression b
    res += emit_expr(b, si );

    // store result w0 into stack
    res += "str w0, [sp, #" + std::to_string(si) + "]\n"; // allocate space on stack
    res += emit_expr(a, si-16); // calc expr a, move back into allocated space

    // load expr b's result
    res += "ldr w1, [sp, #" + std::to_string(si) + "]\n"; 
    // result
    if (e->binary_op == BinaryOpType::Add) {
        res += "add w0, w0, w1\n";
    } 
    else if (e->binary_op == BinaryOpType::Mul) { 
        res += "mul w0, w0, w1\n";
    } else if (e->binary_op == BinaryOpType::Sub) { 
        res += "sub w0, w0, w1\n";
    }

    return res;

}

std::string emit_expr(Expression* e, int si) {
    // emit_expr 
    std::string res;

    // immediate values
    if (e->type == ExpressionType::IntLiteral) {
        res += "mov w0, #" + std::to_string(e->int_val) + "\n"; 
    }else if (e->type == ExpressionType::BoolLiteral) { 
        res += "mov w0, #" + std::to_string(static_cast<int>(e->bool_val)) + "\n";
    } else if (e->type == ExpressionType::CharLiteral) { 
        res += "mov w0, #" + std::to_string((int)e->char_val) + "\n";
    } else if (e->type == ExpressionType::UnaryOp) {
        res += emit_expr(e->operand, si); // recurse and emit asm for the operand (complex expr's result will be in w0 register)
        if (e->unary_op == UnaryOpType::Add1) {
            res += "add w0, w0, #1\n";
        } else if (e->unary_op == UnaryOpType::Sub1) {
            res += "sub w0, w0, #1\n";
        }
    } else if (e->type == ExpressionType::BinaryOp) {
        res += emit_primitive_call(e, si);
    }

    return res;
}


std::string compile_program(Expression* e, std::string func_name = "_c_entry") {
    std::string res;
    res += ".globl " + func_name + "\n"; // asm directives
    res += func_name + ":\n";

    // allocate stack space
    res += "sub sp, sp, #64\n"; // 64 bytes hardcoded for now

    res += emit_expr(e, 48); // 4 stack slots (0,16,32,48), start at 4th

    res += "add sp, sp, #64\n";
    res += "ret\n";
    return res;
}


extern std::vector<Token> lex(const std::string& program);
extern Function* parse_function(std::vector<Token>& tokens);

#ifdef COMPILE_COMPILER_TRUE
int main(int argc, char** argv) {
    if (argc < 2) throw std::runtime_error("Too few arguments.");
    std::string file_name = argv[1];
    std::ifstream ifile{file_name.c_str()};
    std::stringstream buffer;

    buffer << ifile.rdbuf();
    std::string program = buffer.str();
    // std::cout << program << std::endl;

    // lex
    std::vector<Token> tokens = lex(program);

    // parse
    Function* main_func = parse_function(tokens);

    // code gen
    std::string asm_code = compile_program(main_func->body->statements[0]->exp, "_main");

    std::ofstream output_stream("compiled_binaries/" + file_name + ".s");
    output_stream << asm_code;
    
    output_stream.close();

    // run assembler
    std::string cmd = "gcc compiled_binaries/" + file_name + ".s -o compiled_binaries/program";
    std::system(cmd.c_str());
    return 0;
}
#endif
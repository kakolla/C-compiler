#include <fstream>
#include <string>
#include <iostream>


// types of expressions
enum class ExpressionType {
    IntLiteral,
    BoolLiteral,
    CharLiteral,
    UnaryOp,
}; // expressions can be int ilteral, bool literal, etc

enum class UnaryOpType {
    Add1,
    Sub1
}; // types of unary operations (++, --)

struct Expression {
    ExpressionType type;

    // expression payload
    int int_val;
    int bool_val;
    char char_val;

    UnaryOpType unary_op; // unary op specific
    Expression* operand; // to apply unary op on
};



std::string compile_program(int x) {
    std::string res = "mov w0, #" + std::to_string(x) + "\nret\n";
    std::cout << res << std::endl;
    return res;

}

std::string compile_program(char x) {
    return "mov w0, #" + std::to_string((int)x) + "\nret\n";
} 

std::string compile_program(bool x) {
    std::string res = "mov w0, #" + std::to_string(static_cast<int>(x)) + "\nret\n";
    std::cout << res << std::endl;
    return res;
}

std::string emit_expr(Expression* e) {
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
        res += emit_expr(e->operand); // recurse and emit asm for the operand (complex expr's result will be in w0 register)
        if (e->unary_op == UnaryOpType::Add1) {
            res += "add w0, w0, #1\n";
        }
    }

    return res;
}


std::string compile_program(Expression* e) {
    std::string res;
    res += emit_expr(e);
    res += "ret\n";
    return res;
}

int main() {
    // std::string asm_code = compile_program(31415); // generate assembly code in a file
    std::string asm_code = compile_program('K');
    std::ofstream output_stream("c_entry.s");
    output_stream << ".globl _c_entry\n"
        << "_c_entry:\n"
        << asm_code;
    
}
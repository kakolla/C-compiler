#include <fstream>
#include <string>
#include <iostream>


// types of expressions
enum class ExpressionType {
    IntLiteral,
    BoolLiteral,
    CharLiteral,
    UnaryOp,
    BinaryOp
}; // expressions can be int ilteral, bool literal, etc

enum class UnaryOpType {
    Add1,
    Sub1
}; // types of unary operations (++, --)

enum class BinaryOpType {
    Add, 
    Sub,
    Mul
};

struct Expression {
    ExpressionType type;

    // expression payload
    int int_val;
    int bool_val;
    char char_val;

    UnaryOpType unary_op; // unary op specific
    Expression* operand; // to apply unary op on

    BinaryOpType binary_op;
    Expression* operand1;
    Expression* operand2; //  binary_op op1 op2
};

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
        }
    } else if (e->type == ExpressionType::BinaryOp) {
        res += emit_primitive_call(e, si);
    }

    return res;
}


std::string compile_program(Expression* e) {
    std::string res;
    res += ".globl _c_entry\n"; // asm directives
    res += "_c_entry:\n";

    // allocate stack space
    res += "sub sp, sp, #64\n"; // 64 bytes hardcoded for now

    res += emit_expr(e, 48); // 4 stack slots (0,16,32,48), start at 4th

    res += "add sp, sp, #64\n";
    res += "ret\n";
    return res;
}

int main() {
    // std::string asm_code = compile_program(31415); // generate assembly code in a file
    // Expression number = {
    //     .type=ExpressionType::IntLiteral,
    //     .int_val=67
    // };
    // Expression testAST = {
    //     .type=ExpressionType::UnaryOp,
    //     .unary_op=UnaryOpType::Add1,
    //     .operand=&number
    // };

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
        .operand1=&number,
        .operand2=&number2
    };

    std::string asm_code = compile_program(&testAST );
    std::ofstream output_stream("c_entry.s");
    output_stream << asm_code;
    
    output_stream.close();
}
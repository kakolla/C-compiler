// ast expression definitions

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

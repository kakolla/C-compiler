#include <string>
#include <vector>
#include <iostream>
#include "parser.h"
#include "ast.h"

/*
grammar rules:
program -> function
function -> "int" identifier  "(" ")" block
block -> "{" statement "}"
statement -> "return" expression ";"
expression -> sum/product/etc

** product is for operator precedence (its another word for term)
** expression is another word for sum
** factor is the atomic unit

expression -> product + product + ...*
product -> factor * factor *...*
factor -> number | identifier | "(" expression ")" 


*/



size_t i = 0;

void print_expression(Expression* e) {
    if (e == NULL) return;

    switch (e->type) {
        case ExpressionType::IntLiteral:
            std::cout << e->int_val;
            break;
        case ExpressionType::BinaryOp:
            // a <op> b
            std::cout << "(";
            print_expression(e->operand1);
            if (e->binary_op == BinaryOpType::Add) {
                std::cout << " + ";
            } else if (e->binary_op == BinaryOpType::Mul) {
                std::cout << " * ";
            }
            print_expression(e->operand2);
            std::cout << ")";
            break;
        default:
            std::cout << "not implemented printing for this yet";

    }
    
}



Token peek(std::vector<Token>& tokens) {
    if (i >= tokens.size()) return tokens.back();
    return tokens[i];
}

Token expect(std::vector<Token>& tokens, TokenType type) {
    // check if current token matches expected type AND CONSUME it
    if ( i >= tokens.size()) {
        throw std::runtime_error("reached end of input ERROR");
    }

    Token t = tokens[i];
    if (t.type == type) {
        i++; // consume token
        return t;
    } else {
        throw std::runtime_error("Unexpected token, probably bad syntax for this token: " + t.token_val);
    } 
}

Function* parse_function(std::vector<Token>& tokens) {
    // parse function with expected rules
    // build AST
    // ex: int main()
    expect(tokens, TokenType::KeywordInt);
    std::string func_name = expect(tokens, TokenType::Identifier).token_val;
    expect(tokens, TokenType::LParen);
    expect(tokens, TokenType::RParen);
    // ast node
    Function* func_node = new Function();
    func_node->name = func_name;
    func_node->body = parse_block(tokens); // recursively parse block, which creates the children nodes

    return func_node;
}

Block* parse_block(std::vector<Token>& tokens) {
    // build ast node for a block
    // ex: { statement + statement + ...* }
    expect(tokens, TokenType::LBrace); // consume {
    Block* block_node = new Block();
    // TODO: extend to multiple statements
    Statement* st_node = parse_statement(tokens); // recursively parse statement
    block_node->statements.push_back(st_node);
    expect(tokens, TokenType::RBrace); // consume }

    return block_node;
}

Statement* parse_statement(std::vector<Token>& tokens) {
    // statement AST node (ie return statement)
    // return <expression> ;

    Statement* st_node = new Statement();
    expect(tokens, TokenType::KeywordReturn); // consume return keyword
    Expression* expr_node = parse_expression(tokens);
    st_node->exp = expr_node;
    st_node->type = StatementType::Return; // return statement


    expect(tokens, TokenType::Semicolon); // consume ;

    return st_node;
}


Expression* parse_expression(std::vector<Token>& tokens) {
    // expression -> product + product + ...*
    // ex: exp_prod + exp_prod

    // get lhs node (recursively)
    Expression* lhs = parse_product(tokens);
    while (i < tokens.size() && peek(tokens).type == TokenType::Plus) {
        i++; // consume  +

        // adding up all products
        Expression* rhs = parse_product(tokens);

        Expression* parent_node = new Expression();
        parent_node->type = ExpressionType::BinaryOp;
        parent_node->binary_op = BinaryOpType::Add;
        parent_node->operand1 = lhs;
        parent_node->operand2 = rhs;
        
        lhs = parent_node; // results get summed left to right, next parent node's left child is this sum        
        // accumulates it
    }

    return lhs;
}

Expression* parse_product(std::vector<Token>& tokens) {
    // create product AST node
    // called from parse expression recursively (so operator precedence)
    // ex: product -> factor * factor * ...*
    // ex: (27) * (27) * (27)
    Expression* lhs = parse_factor(tokens);

    while (i < tokens.size() && peek(tokens).type == TokenType::Mul) {
        i++; // consume *

        // multiply the expressions (which have factors)
        Expression* rhs = parse_factor(tokens);

        Expression* parent_node = new Expression();
        parent_node->type = ExpressionType::BinaryOp;
        parent_node->binary_op = BinaryOpType::Mul;
        parent_node->operand1 = lhs;
        parent_node->operand2 = rhs;

        lhs = parent_node;
    }
    return lhs;
}

Expression* parse_factor(std::vector<Token>& tokens) {
    // factor -> num | identifier | "(" expression ")"
    Expression* factor_node;
    if (peek(tokens).type == TokenType::IntLiteral) {
        factor_node = new Expression();
        factor_node->type = ExpressionType::IntLiteral;
        factor_node->int_val = std::stoi(expect(tokens, TokenType::IntLiteral).token_val);
        return factor_node;
    // } else if (peek(tokens).type == TokenType::Identifier) {
        // factor_node = new Expression();
        // factor_node->type = ExpressionType::;
        // factor_node->int_val = std::stoi(expect(tokens, TokenType::IntLiteral).token_val);
        // return factor_node;  
        // TODO: not implemented yet
    } else if (peek(tokens).type == TokenType::LParen) {
        expect(tokens, TokenType::LParen);//consume (

        // get expression node
        factor_node = parse_expression(tokens);
        expect(tokens, TokenType::RParen);//consume )
        
    } else {
        throw std::runtime_error("Factor not identified as one of numer, identifier, or another expression: " + peek(tokens).token_val);
    }

    return factor_node; 
}


extern std::vector<Token> lex(const std::string& program);

// int main() {
//     // std::vector<Token> tokens = {{TokenType::IntLiteral, "3"},{TokenType::Mul, "*"},{TokenType::IntLiteral, "5"} };
//     std::vector<Token> tokens = lex("int main() {return 5*3;} ");
//     for (auto e : tokens) {
//         std::cout << e.token_val << std::endl;
//     }

//     i = 0;
//     Function* result = parse_function(tokens);
//     print_expression(result->body->statements[0]->exp);
//     std::cout << std::endl;
    
//     return 0;
// }
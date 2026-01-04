#ifndef parser_h
#define parser_h


#include <string>
#include <vector>
#include "ast.h"

enum class TokenType {
    LBrace, // {
    RBrace, // }
    LParen, // (
    RParen,  // )
    Semicolon, // ;
    Mul,
    Plus,
    KeywordInt, // int
    KeywordReturn, // return
    Identifier, // names, like main, x
    IntLiteral, // 999
    EndOfFile
};

struct Token {
    TokenType type;
    std::string token_val;
};

// token traversal funcs
Token peek(std::vector<Token>& tokens);
Token expect(std::vector<Token>& tokens, TokenType type);

Function* parse_function(std::vector<Token>& tokens);
Block* parse_block(std::vector<Token>& tokens);

Statement* parse_statement(std::vector<Token>& tokens);
Expression* parse_expression(std::vector<Token>& tokens);
Expression* parse_product(std::vector<Token>& tokens);
Expression* parse_factor(std::vector<Token>& tokens);

#endif
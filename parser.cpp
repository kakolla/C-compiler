#include <string>
#include <vector>
#include <iostream>


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

size_t i = 0;
/* parser related nodes*/

int parse_sum(std::vector<Token>& tokens);
int parse_product(std::vector<Token>& tokens);
int parse_factor(std::vector<Token>& tokens);


int parse_sum(std::vector<Token>& tokens) { 
    int lhs = parse_product(tokens);
    while (i < tokens.size() && tokens[i].type == TokenType::Plus) {
        ++i; // consume +
        int rhs = parse_product(tokens);
        lhs += rhs;
    }
    return lhs;
}


int parse_product(std::vector<Token>& tokens) {
    int lhs = parse_factor(tokens);
    while (i < tokens.size() && tokens[i].type == TokenType::Mul) {
        ++i; // consume *
        int rhs = parse_factor(tokens);
        lhs *= rhs;
    }
    return lhs;

}

int parse_factor(std::vector<Token>& tokens) {
    Token t = tokens[i];

    // assuming these are numbers and the token is not empty
    if (t.type == TokenType::IntLiteral) {
        i++; // consume literal
        return std::stoi(t.token_val);   
    } else if (t.type == TokenType::LParen) {
        i++; // consume (
        int val = parse_sum(tokens);
        i++; // consume )
        return val;
    } else {
        std::cout << "token  isn't a number" << t.token_val << std::endl;
        throw std::runtime_error("expected a factor");
    }
}



void parse(std::vector<Token>& tokens) {



}


int main() {
    std::vector<Token> tokens = {{TokenType::IntLiteral, "3"},{TokenType::Mul, "*"},{TokenType::IntLiteral, "5"} };
    int result = parse_product(tokens);
    std::cout << result << std::endl;
    
    return 0;
}
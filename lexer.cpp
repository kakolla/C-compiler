

#include <string>
#include <iostream>
#include <vector>
#include "parser.h"

using std::cout;
using std::endl;


std::vector<Token> lex(const std::string& program) {
    std::vector<Token> tokens;
    // scan through program for tokens
    size_t i =0;
    while (i < program.size() ) {
        char c = program[i];
        if (isspace(c)) i++;
        else if (c == '{') {
            tokens.push_back({TokenType::LBrace, "{"});
            i++;
        } else if (c == '}') {
            tokens.push_back({TokenType::RBrace, "}"});
            i++;
        } else if (c == '(') {
            tokens.push_back({TokenType::LParen, "("});
            i++;
        } else if (c == ')') {
            tokens.push_back({TokenType::RParen, ")"});
            i++;
        } else if (c == ';') {
            tokens.push_back({TokenType::Semicolon, ";"});
            i++;
        } else if (c == '*') {
            tokens.push_back({TokenType::Mul, "*"});
            i++;  
        }
        else if (c == '+') {
            tokens.push_back({TokenType::Plus, "+"});
            i++;  
        }
        else if (isdigit(c))  {
            //  read digits
            std::string temp;
            while (i < program.size() && isdigit(program[i])) {
                temp.push_back(program[i]);
                i++;
            }
            tokens.push_back({TokenType::IntLiteral, temp});
        } else if (isalpha(c) || c == '_') {
            // read identifier/keyword (int, return, or identifier)
            std::string temp;
            while (i < program.size() && (isalnum(program[i]) || program[i] == '_')) {
                // if character is alphanumeric or has _
                temp.push_back(program[i]);
                i++;
            }
            // check for identifiers
            if (temp == "int") {
                tokens.push_back({TokenType::KeywordInt, temp});
            } else if (temp == "return") {
                tokens.push_back({TokenType::KeywordReturn, temp});
            } else {
                tokens.push_back({TokenType::Identifier, temp});
            }
        } else {
            // error
            // TODO: return error to user
            tokens.clear();
            return tokens; 
        }

    }

    tokens.push_back({TokenType::EndOfFile, ""});

    return tokens;
}


// int main() {

//     std::vector<Token> t = lex("int main() milk;");
    
//     for (auto e : t) {
//         std::cout << e.token_val << std::endl;
//     }
//     return 0;
// }
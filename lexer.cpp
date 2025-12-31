

#include <string>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;


enum class TokenType {
    LBrace, // {
    RBrace, // }
    LParen, // (
    RParen,  // )
    Semi, // ;
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

std::vector<Token> lex(std::string program) {
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
            tokens.push_back({TokenType::Semi, ";"});
            i++;
        } else if (isdigit(c))  {
            // TODO: read digits
            i++;
        } else if (isalpha(c)) {
            // TODO: read identifier/keyword (int, return, or identifier)
            i++;
        } else {
            // error
            tokens.clear();
            return tokens; 
        }

    }

    tokens.push_back({TokenType::EndOfFile, ""});

    return tokens;
}


int main() {

    std::vector<Token> t = lex("int main() milk;");
    for (auto e : t) {
        cout << e.token_val << endl;
    }
    return 0;
}
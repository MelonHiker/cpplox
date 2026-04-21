#pragma once

#include <unordered_map>
#include <vector>

#include "Token.hpp"

class Scanner {
   private:
    const std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    static const std::unordered_map<std::string, TokenType> keywords;

    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    void parseString();
    void parseNumber();
    void parseIdentifier();
    void scanToken();
    void addToken(TokenType type, Literal literal = std::monostate{});

   public:
    Scanner(std::string source);
    std::vector<Token> scanTokens();
};
#pragma once
#include <string>
#include <variant>

#include "TokenType.hpp"

using Literal = std::variant<std::string, double, std::monostate>;

class Token {
   public:
    TokenType type;
    std::string lexeme;
    Literal literal;
    int line;

    Token(TokenType type, std::string lexeme, Literal literal, int line);
    std::string toString() const;
};
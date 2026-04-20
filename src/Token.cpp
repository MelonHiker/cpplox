#include "Token.hpp"

#include <string>
#include <variant>

#include "Util.hpp"

class Token {
    const TokenType type;
    const std::string lexeme;
    const Literal literal;
    const int line;

    Token(TokenType type, std::string lexeme, Literal literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}

   public:
    std::string toString() {
        std::string literalString;
        literalString = std::visit(overloaded{[](const std::string& s) { return s; },
                                              [](const double d) { return std::to_string(d); },
                                              [](std::monostate) { return "null"; }},
                                   literal);
        return typeToString(type) + " " + lexeme + " " + literalString;
    }
};
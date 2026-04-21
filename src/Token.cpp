#include "Token.hpp"

#include <string>
#include <variant>

#include "Util.hpp"

Token::Token(TokenType type, std::string lexeme, Literal literal, int line)
    : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

std::string Token::toString() const {
    std::string literalString =
        std::visit(overloaded{[](const std::string& s) { return s; },
                              [](double d) { return std::to_string(d); },
                              [](std::monostate) { return std::string("null"); }},
                   literal);

    return typeToString(type) + " " + lexeme + " " + literalString;
}

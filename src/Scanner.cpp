#include "Scanner.hpp"

#include <cctype>

#include "Lox.hpp"

const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and", TokenType::AND},     {"class", TokenType::CLASS},   {"else", TokenType::ELSE},
    {"false", TokenType::FALSE}, {"for", TokenType::FOR},       {"fun", TokenType::FUN},
    {"if", TokenType::IF},       {"nil", TokenType::NIL},       {"or", TokenType::OR},
    {"print", TokenType::PRINT}, {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
    {"this", TokenType::THIS},   {"true", TokenType::TRUE},     {"var", TokenType::VAR},
    {"while", TokenType::WHILE}};

Scanner::Scanner(std::string source) : source(std::move(source)) {}

bool Scanner::isAtEnd() const { return current >= static_cast<int>(source.size()); }

char Scanner::advance() {
    if (isAtEnd()) {
        return '\0';
    }
    return source[current++];
}

char Scanner::peek() const {
    if (isAtEnd()) {
        return '\0';
    }
    return source[current];
}

char Scanner::peekNext() const {
    if (current + 1 >= static_cast<int>(source.size())) {
        return '\0';
    }
    return source[current + 1];
}

bool Scanner::match(char expected) {
    if (isAtEnd() || source[current] != expected) {
        return false;
    }
    current++;
    return true;
}

void Scanner::parseString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
        }
        advance();
    }

    if (isAtEnd()) {
        Lox::error(line, "Unterminated string.");
        return;
    }

    advance();

    std::string text = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, text);
}

void Scanner::parseNumber() {
    while (std::isdigit(static_cast<unsigned char>(peek()))) {
        advance();
    }

    if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peekNext()))) {
        advance();
        while (std::isdigit(static_cast<unsigned char>(peek()))) {
            advance();
        }
    }

    addToken(TokenType::NUMBER, std::stod(source.substr(start, current - start)));
}

void Scanner::parseIdentifier() {
    while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
        advance();
    }

    std::string text = source.substr(start, current - start);
    auto keyword = keywords.find(text);
    if (keyword != keywords.end()) {
        addToken(keyword->second);
    } else {
        addToken(TokenType::IDENTIFIER);
    }
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(':
            addToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            addToken(TokenType::RIGHT_PAREN);
            break;
        case '{':
            addToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            addToken(TokenType::RIGHT_BRACE);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case '.':
            addToken(TokenType::DOT);
            break;
        case '-':
            addToken(TokenType::MINUS);
            break;
        case '+':
            addToken(TokenType::PLUS);
            break;
        case ';':
            addToken(TokenType::SEMICOLON);
            break;
        case '*':
            addToken(TokenType::STAR);
            break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) {
                    advance();
                }
            } else {
                addToken(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '"':
            parseString();
            break;
        default:
            if (std::isdigit(static_cast<unsigned char>(c))) {
                parseNumber();
            } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                parseIdentifier();
            } else {
                Lox::error(line, "Unexpected character.");
            }
            break;
    }
}

void Scanner::addToken(TokenType type, Literal literal) {
    std::string lexeme = source.substr(start, current - start);
    tokens.emplace_back(type, lexeme, literal, line);
}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::EOF_TOKEN, "", std::monostate{}, line);
    return tokens;
}

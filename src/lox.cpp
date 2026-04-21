#include "Lox.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "Scanner.hpp"

bool Lox::hadError = false;

void Lox::main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: lox [script]" << '\n';
        std::exit(64);
    }

    if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }
}

void Lox::error(int line, std::string message) { report(line, "", message); }

void Lox::runFile(const std::string& path) {
    std::ifstream file(path);
    std::string source{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    run(source);

    if (hadError) {
        std::exit(65);
    }
}

void Lox::runPrompt() {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line) || line.empty()) {
            break;
        }

        run(line);
        hadError = false;
    }
}

void Lox::run(const std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    for (const Token& token : tokens) {
        std::cout << token.toString() << '\n';
    }
}

void Lox::report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << '\n';
    hadError = true;
}

int main(int argc, char* argv[]) {
    Lox::main(argc, argv);
    return 0;
}

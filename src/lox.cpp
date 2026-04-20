#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

class Lox {
   public:
    inline static bool hadError = false;
    static void main(int argc, char* argv[]) {
        if (argc > 1) {
            std::cout << "Usage: jlox [script]";
            std::exit(64);
        } else if (argc == 1) {
            runFile(argv[0]);
        } else {
            runPrompt();
        }
    }
    static void error(int line, std::string message) { report(line, "", message); }

   private:
    static void runFile(std::string path) {
        std::ifstream t(path);
        std::string source{std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>()};
        run(source);
        if (hadError) std::exit(65);
    }
    static void runPrompt() {
        std::cout << "> \n";
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                break;
            }
            run(line);
            hadError = false;
        }
    }
    static void run(std::string source) {
        Scanner scanner(source);
        vector<Token> tokens = scanner.scanTokens();
        for (Token token : tokens) {
            std::cout << token << '\n';
        }
    }
    static void report(int line, std::string where, std::string message) {
        std::cerr << "[line " + std::to_string(line) + "] Error" + where + ": " + message << '\n';
    }
};

int main(int argc, char* argv[]) {
    Lox::main(argc, argv);
    return 0;
}
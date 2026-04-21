#pragma once

#include <string>

class Lox {
   public:
    static bool hadError;
    static void main(int argc, char* argv[]);
    static void error(int line, std::string message);

   private:
    static void runFile(const std::string& path);
    static void runPrompt();
    static void run(const std::string& source);
    static void report(int line, const std::string& where, const std::string& message);
};
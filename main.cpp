#include "include/lexer/Lexer.h"
#include "include/parser/Parser.h"
#include "include/interpreter/Interpreter.h"
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Invalid number of arguments" << std::endl;
        return 1;
    }

    std::fstream sourceFile(argv[1]);

    if (!sourceFile.is_open())
    {
        std::cerr << "Cannot open file" << std::endl;
        return 1;
    }

    Lexer lexer(sourceFile);
    Parser parser(lexer);
    Interpreter interpreter;
    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    try
    {
        program->accept(interpreter);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        sourceFile.close();
        return 1;
    }

    sourceFile.close();

    return 0;
}
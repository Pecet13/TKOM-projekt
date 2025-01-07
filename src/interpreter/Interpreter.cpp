#include "interpreter/Interpreter.h"

void Interpreter::visit(const ProgramNode& node)
{
    for (const auto& decl : node.getDeclarations())
    {
        decl->accept(*this);
    }
}

void Interpreter::visit(const FunctionDeclarationNode& node)
{
    if (functions.find(node.getIdentifier()) != functions.end())
    {
        throw InterpreterException("Duplicate function declaration: " + node.getIdentifier());
    }
    functions[node.getIdentifier()] = &node;
}
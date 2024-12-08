#ifndef VARIABLE_DECLARATION_NODE_H
#define VARIABLE_DECLARATION_NODE_H

#include "DeclarationNode.h"
#include "ExpressionNode.h"
#include <string>
#include <memory>

class VariableDeclarationNode : public DeclarationNode
{
private:
    bool isMutable;
    std::string type;
    std::string identifier;
    std::unique_ptr<ExpressionNode> initializer;

public:
    VariableDeclarationNode(bool isMutable, const std::string& type, 
                            const std::string& identifier, std::unique_ptr<ExpressionNode> initializer = nullptr)
        : isMutable(isMutable), type(type), identifier(identifier), initializer(std::move(initializer)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "VariableDeclaration("+ (isMutable ? "mut, " : "") + "type: " + type + ", identifier: " + identifier + ")\n";
        if (initializer)
        {
            result += "\n" + initializer->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
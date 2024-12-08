#ifndef FUNCTION_DECLARATION_NODE_H
#define FUNCTION_DECLARATION_NODE_H

#include "DeclarationNode.h"
#include "ParameterListNode.h"
#include "BlockNode.h"
#include <string>
#include <vector>
#include <memory>

class FunctionDeclarationNode : public DeclarationNode
{
private:
    std::string returnType;
    std::string name;
    std::unique_ptr<ParameterListNode> parameters;
    std::unique_ptr<BlockNode> block;

public:
    FunctionDeclarationNode(const std::string& returnType, const std::string& name,
                            std::unique_ptr<ParameterListNode> parameters, std::unique_ptr<BlockNode> block)
        : returnType(returnType), name(name), parameters(std::move(parameters)), block(std::move(block)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "FunctionDeclaration(returnType: " + returnType + ", name: " + name + ")\n" +
               parameters->toString(indentLevel + 1) + "\n" +
               block->toString(indentLevel + 1);
    }
};

#endif
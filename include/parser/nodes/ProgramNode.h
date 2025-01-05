#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H

#include "DeclarationNode.h"
#include <vector>
#include <memory>

class ProgramNode : public Node
{
private:
    std::vector<std::unique_ptr<DeclarationNode>> declarations;

public:
    void addDeclaration(std::unique_ptr<DeclarationNode> declaration)
    {
        declarations.push_back(std::move(declaration));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Program\n";
        for (const auto& decl : declarations)
        {
            result += decl->toString(indentLevel + 1);
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

#endif
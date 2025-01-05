#ifndef STRUCT_DECLARATION_NODE_H
#define STRUCT_DECLARATION_NODE_H

#include "DeclarationNode.h"
#include "StatementNode.h"
#include "StructFieldListNode.h"
#include <string>
#include <vector>
#include <memory>

class StructDeclarationNode : public DeclarationNode, public StatementNode
{
private:
    std::string identifier;
    std::unique_ptr<StructFieldListNode> fields;

public:
    StructDeclarationNode(const std::string& identifier, std::unique_ptr<StructFieldListNode> fields)
        : identifier(identifier), fields(std::move(fields)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "StructDeclaration(identifier: " + identifier + ")\n" + fields->toString(indentLevel + 1);
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

#endif
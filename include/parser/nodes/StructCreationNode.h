#ifndef STRUCT_CREATION_NODE_H
#define STRUCT_CREATION_NODE_H

#include "StatementNode.h"
#include "ArgumentListNode.h"
#include <string>
#include <memory>

class StructCreationNode : public StatementNode
{
private:
    std::string type;
    std::string identifier;
    std::unique_ptr<ArgumentListNode> arguments;

public:
    StructCreationNode(const std::string& type, const std::string& identifier, std::unique_ptr<ArgumentListNode> arguments)
        : type(type), identifier(identifier), arguments(std::move(arguments)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "StructCreation(type: " + type + ", identifier: " + identifier + ")\n" +
                arguments->toString(indentLevel + 1);
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

#endif
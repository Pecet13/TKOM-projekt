#ifndef STRUCT_CREATION_NODE_H
#define STRUCT_CREATION_NODE_H

#include "Node.h"
#include <string>
#include <memory>

class StructCreationNode : public Node
{
private:
    std::string type;
    std::string identifier;
    std::unique_ptr<Node> arguments;

public:
    StructCreationNode(const std::string& type, const std::string& identifier, std::unique_ptr<Node> arguments)
        : type(type), identifier(identifier), arguments(std::move(arguments)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "StructCreationNode(type: " + type + ", identifier: " + identifier + "\n" +
                arguments->toString(indentLevel + 1);
    }
};

#endif
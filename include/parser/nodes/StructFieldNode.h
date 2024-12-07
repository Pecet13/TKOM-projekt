#ifndef STRUCT_FIELD_NODE_H
#define STRUCT_FIELD_NODE_H

#include "Node.h"
#include <string>

class StructFieldNode : public Node
{
private:
    bool isMutable;
    std::string type;
    std::string identifier;

public:
    StructFieldNode(bool isMutable, const std::string& type, const std::string& identifier)
        : isMutable(isMutable), type(type), identifier(identifier) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "StructFieldNode(" + (isMutable ? "mut, " : "") +
               "type: " + type + ", identifier: " + identifier + ")\n";
    }
};

#endif
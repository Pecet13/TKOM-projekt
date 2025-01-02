#ifndef STRING_LITERAL_NODE_H
#define STRING_LITERAL_NODE_H

#include "LiteralNode.h"
#include <string>

class StringLiteralNode : public LiteralNode
{
private:
    std::string value;

public:
    StringLiteralNode(const std::string& value)
        : value(value) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "StringLiteral(value: \"" + value + "\")\n";
    }
};

#endif
#ifndef INT_LITERAL_NODE_H
#define INT_LITERAL_NODE_H

#include "LiteralNode.h"

class IntLiteralNode : public LiteralNode
{
private:
    int value;

public:
    IntLiteralNode(int value)
        : value(value) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "IntLiteral(value: " + std::to_string(value) + ")\n";
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    int getValue() const
    {
        return value;
    }
};

#endif
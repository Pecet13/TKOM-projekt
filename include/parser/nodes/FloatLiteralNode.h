#ifndef FLOAT_LITERAL_NODE_H
#define FLOAT_LITERAL_NODE_H

#include "LiteralNode.h"

class FloatLiteralNode : public LiteralNode
{
private:
    float value;

public:
    FloatLiteralNode(float value)
        : value(value) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "FloatLiteral(value: " + std::to_string(value) + ")\n";
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    float getValue() const
    {
        return value;
    }
};

#endif
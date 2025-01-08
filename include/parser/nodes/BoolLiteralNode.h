#ifndef BOOL_LITERAL_NODE_H
#define BOOL_LITERAL_NODE_H

#include "LiteralNode.h"

class BoolLiteralNode : public LiteralNode
{
private:
    bool value;

public:
    BoolLiteralNode(bool value)
        : value(value) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "BoolLiteral(value: " + std::string(value ? "true" : "false") + ")\n";
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    bool getValue() const
    {
        return value;
    }
};

#endif
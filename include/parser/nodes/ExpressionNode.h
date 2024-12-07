#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H

#include "Node.h"
#include <vector>
#include <memory>

class ExpressionNode : public Node
{
private:
    std::unique_ptr<Node> left;
    std::vector<std::unique_ptr<Node>> rights;

public:
    ExpressionNode(std::unique_ptr<Node> left)
        : left(std::move(left)) {}
    
    void addRight(std::unique_ptr<Node> right)
    {
        rights.push_back(std::move(right));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "ExpressionNode\n";
        result += left->toString(indentLevel + 1);
        for (const auto& right : rights)
        {
            result += indent + "-or\n" + right->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
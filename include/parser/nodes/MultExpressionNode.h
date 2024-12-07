#ifndef MULT_EXPRESSION_NODE_H
#define MULT_EXPRESSION_NODE_H

#include "Node.h"
#include <string>
#include <vector>
#include <memory>

class MultExpressionNode : public Node
{
private:
    std::unique_ptr<Node> left;
    std::vector<std::pair<std::string, std::unique_ptr<Node>>> rights;

public:
    MultExpressionNode(std::unique_ptr<Node> left)
        : left(std::move(left)) {}

    void addRight(const std::string& op, std::unique_ptr<Node> right)
    {
        rights.emplace_back(op, std::move(right));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "MultExpressionNode\n";
        result += left->toString(indentLevel + 1);
        for (const auto& pair : rights)
        {
            result += indent + "-" + pair.first + "\n" + pair.second->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
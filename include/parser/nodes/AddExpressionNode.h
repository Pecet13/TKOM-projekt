#ifndef ADD_EXPRESSION_NODE_H
#define ADD_EXPRESSION_NODE_H

#include "Node.h"
#include "MultExpressionNode.h"
#include <string>
#include <vector>
#include <memory>

class AddExpressionNode : public Node
{
private:
    std::unique_ptr<MultExpressionNode> left;
    std::vector<std::pair<std::string, std::unique_ptr<MultExpressionNode>>> rights;

public:
    AddExpressionNode(std::unique_ptr<MultExpressionNode> left)
        : left(std::move(left)) {}

    void addRight(const std::string& op, std::unique_ptr<MultExpressionNode> right)
    {
        rights.emplace_back(op, std::move(right));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "AddExpressionNode\n";
        result += left->toString(indentLevel + 1);
        for (const auto& pair : rights)
        {
            result += indent + "-" + pair.first + "\n" + pair.second->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
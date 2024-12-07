#ifndef COMPARISON_NODE_H
#define COMPARISON_NODE_H

#include "Node.h"
#include <string>
#include <vector>
#include <memory>

class ComparisonNode : public Node
{
private:
    std::unique_ptr<Node> left;
    std::string op;
    std::unique_ptr<Node> right;

public:
    ComparisonNode(std::unique_ptr<Node> left, const std::string& op, std::unique_ptr<Node> right = nullptr)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "ComparisonNode\n";
        result += left->toString(indentLevel + 1);
        if (right)
        {
            result += indent + "-" + op + "\n" + right->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
#ifndef COMPARISON_NODE_H
#define COMPARISON_NODE_H

#include "AddExpressionNode.h"
#include <string>
#include <vector>
#include <memory>

enum class ComparisonOperator
{
    NONE,
    EQUAL,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL
};

class ComparisonNode : public Node
{
private:
    std::unique_ptr<AddExpressionNode> left;
    ComparisonOperator op;
    std::unique_ptr<AddExpressionNode> right;

public:
    ComparisonNode(std::unique_ptr<AddExpressionNode> left, ComparisonOperator op = ComparisonOperator::NONE,
                    std::unique_ptr<AddExpressionNode> right = nullptr)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Comparison\n";
        result += left->toString(indentLevel + 1);
        if (right)
        {
            result += indent + "-operator: ";
            switch (op)
            {
                case ComparisonOperator::NONE: 
                    result += "none";
                    break;
                case ComparisonOperator::EQUAL:
                    result += "==";
                    break;
                case ComparisonOperator::NOT_EQUAL:
                    result += "!=";
                    break;
                case ComparisonOperator::GREATER:
                    result += ">";
                    break;
                case ComparisonOperator::GREATER_EQUAL:
                    result += ">=";
                    break;
                case ComparisonOperator::LESS:
                    result += "<";
                    break;
                case ComparisonOperator::LESS_EQUAL:
                    result += "<=";
                    break;
                default:
                    result += "unknown";
                    break;
            }
            result += "\n" + right->toString(indentLevel + 1);
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    const AddExpressionNode* getLeft() const
    {
        return left.get();
    }

    ComparisonOperator getOp() const
    {
        return op;
    }

    const AddExpressionNode* getRight() const
    {
        return right.get();
    }
};

#endif
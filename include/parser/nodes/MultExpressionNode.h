#ifndef MULT_EXPRESSION_NODE_H
#define MULT_EXPRESSION_NODE_H

#include "TermNode.h"
#include <string>
#include <vector>
#include <memory>

enum class MultOperator
{
    ASTERISK,
    SLASH
};

class MultExpressionNode : public Node
{
private:
    std::unique_ptr<TermNode> left;
    std::vector<std::pair<MultOperator, std::unique_ptr<TermNode>>> rights;

public:
    MultExpressionNode(std::unique_ptr<TermNode> left)
        : left(std::move(left)) {}

    void addRight(MultOperator op, std::unique_ptr<TermNode> right)
    {
        rights.emplace_back(op, std::move(right));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "MultExpression\n";
        result += left->toString(indentLevel + 1);
        for (const auto& pair : rights)
        {
            result += indent + "-operator: ";
            MultOperator op = pair.first;
            switch (op)
            {
                case MultOperator::ASTERISK:
                    result += "*";
                    break;
                case MultOperator::SLASH:
                    result += "/";
                    break;
                default:
                    result += "unknown";
                    break;
            } 
            result += "\n" + pair.second->toString(indentLevel + 1);
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    const TermNode* getLeft() const
    {
        return left.get();
    }

    const std::vector<std::pair<MultOperator, std::unique_ptr<TermNode>>>& getRights() const
    {
        return rights;
    }
};

#endif
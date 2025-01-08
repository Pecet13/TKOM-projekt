#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H

#include "AndExpressionNode.h"
#include <vector>
#include <memory>

class ExpressionNode : public Node
{
private:
    std::unique_ptr<AndExpressionNode> left;
    std::vector<std::unique_ptr<AndExpressionNode>> rights;

public:
    ExpressionNode(std::unique_ptr<AndExpressionNode> left)
        : left(std::move(left)) {}
    
    void addRight(std::unique_ptr<AndExpressionNode> right)
    {
        rights.push_back(std::move(right));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Expression\n";
        result += left->toString(indentLevel + 1);
        for (const auto& right : rights)
        {
            result += indent + "-or\n" + right->toString(indentLevel + 1);
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    const AndExpressionNode* getLeft() const
    {
        return left.get();
    }

    const std::vector<std::unique_ptr<AndExpressionNode>>& getRights() const
    {
        return rights;
    }
};

#endif
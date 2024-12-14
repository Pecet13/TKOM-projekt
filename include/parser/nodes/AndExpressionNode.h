#ifndef AND_EXPRESSION_NODE_H
#define AND_EXPRESSION_NODE_H

#include "ComparisonNode.h"
#include <vector>
#include <memory>

class AndExpressionNode : public Node
{
private:
    std::unique_ptr<ComparisonNode> left;
    std::vector<std::unique_ptr<ComparisonNode>> rights;

public:
    AndExpressionNode(std::unique_ptr<ComparisonNode> left)
        : left(std::move(left)) {}
    
    void addRight(std::unique_ptr<ComparisonNode> right)
    {
        rights.push_back(std::move(right));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "AndExpression\n";
        result += left->toString(indentLevel + 1);
        for (const auto& right : rights)
        {
            result += indent + "-and\n" + right->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
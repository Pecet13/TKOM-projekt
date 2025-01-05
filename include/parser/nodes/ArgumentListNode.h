#ifndef ARGUMENT_LIST_NODE_H
#define ARGUMENT_LIST_NODE_H

#include "ExpressionNode.h"
#include <vector>
#include <memory>

class ArgumentListNode : public Node
{
private:
    std::vector<std::unique_ptr<ExpressionNode>> arguments;

public:
    void addArgument(std::unique_ptr<ExpressionNode> argument)
    {
        arguments.push_back(std::move(argument));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "ArgumentList\n";
        for (const auto& arg : arguments)
        {
            result += arg->toString(indentLevel + 1);
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

#endif
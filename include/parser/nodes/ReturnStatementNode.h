#ifndef RETURN_STATEMENT_NODE_H
#define RETURN_STATEMENT_NODE_H

#include "StatementNode.h"
#include "ExpressionNode.h"
#include <memory>

class ReturnStatementNode : public StatementNode
{
private:
    std::unique_ptr<ExpressionNode> expression;

public:
    ReturnStatementNode(std::unique_ptr<ExpressionNode> expression = nullptr)
        : expression(std::move(expression)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "ReturnStatement\n";
        if (expression)
        {
            result += expression->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
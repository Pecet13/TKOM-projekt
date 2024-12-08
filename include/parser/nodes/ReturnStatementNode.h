#ifndef RETURN_STATEMENT_NODE_H
#define RETURN_STATEMENT_NODE_H

#include "StatementNode.h"
#include "ExpressionNode.h"
#include <memory>

class ReturnStatementNode : public StatementNode
{
private:
    std::unique_ptr<ExpressionNode> returnValue;

public:
    ReturnStatementNode(std::unique_ptr<ExpressionNode> returnValue)
        : returnValue(std::move(returnValue)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "ReturnStatement\n" + returnValue->toString(indentLevel + 1);
    }
};

#endif
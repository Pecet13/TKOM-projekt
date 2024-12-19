#ifndef ASSIGNMENT_NODE_H
#define ASSIGNMENT_NODE_H

#include "StatementNode.h"
#include "FieldOrFunCallNode.h"
#include "ExpressionNode.h"
#include <memory>
#include <string>

class AssignmentNode : public StatementNode
{
private:
    std::unique_ptr<FieldOrFunCallNode> fieldOrFunCall;
    std::unique_ptr<ExpressionNode> expression;

public:
    AssignmentNode(std::unique_ptr<FieldOrFunCallNode> fieldOrFunCall, std::unique_ptr<ExpressionNode> expression = nullptr)
        : fieldOrFunCall(std::move(fieldOrFunCall)), expression(std::move(expression)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Assignment\n" + fieldOrFunCall->toString(indentLevel + 1);
        if (expression)
        {
            result += expression->toString(indentLevel + 1);
        }
        return result;     
    }
};

#endif
#ifndef ASSIGNMENT_NODE_H
#define ASSIGNMENT_NODE_H

#include "StatementNode.h"
#include <memory>
#include <string>

class AssignmentNode : public StatementNode
{
private:
    std::string identifier;
    std::unique_ptr<Node> expression;

public:
    AssignmentNode(const std::string& identifier, std::unique_ptr<Node> expression)
        : identifier(identifier), expression(std::move(expression)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "Assignment(identifier: " + identifier + ")\n" +
               expression->toString(indentLevel + 1);
    }
};

#endif
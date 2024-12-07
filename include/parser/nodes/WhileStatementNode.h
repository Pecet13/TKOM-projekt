#ifndef WHILE_STATEMENT_NODE_H
#define WHILE_STATEMENT_NODE_H

#include "StatementNode.h"
#include <memory>

class WhileStatementNode : public StatementNode
{
private:
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> block;

public:
    WhileStatementNode(std::unique_ptr<Node> condition, std::unique_ptr<Node> block)
        : condition(std::move(condition)), block(std::move(block)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "WhileStatement\n" +
               condition->toString(indentLevel + 1) + "\n" +
               block->toString(indentLevel + 1);
    }
};

#endif
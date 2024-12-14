#ifndef BLOCK_NODE_H
#define BLOCK_NODE_H

#include "StatementNode.h"
#include <vector>
#include <memory>

class BlockNode : public Node
{
private:
    std::vector<std::unique_ptr<StatementNode>> statements;

public:
    void addStatement(std::unique_ptr<StatementNode> statement)
    {
        statements.push_back(std::move(statement));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Block\n";
        for (const auto& stmt : statements)
        {
            result += stmt->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
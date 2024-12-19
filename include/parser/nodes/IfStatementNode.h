#ifndef IF_STATEMENT_NODE_H
#define IF_STATEMENT_NODE_H

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "BlockNode.h"
#include <memory>

class IfStatementNode : public StatementNode
{
private:
    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockNode> block;
    std::unique_ptr<BlockNode> elseBlock;

public:
    IfStatementNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<BlockNode> block,
                    std::unique_ptr<BlockNode> elseBlock = nullptr)
        : condition(std::move(condition)), block(std::move(block)), elseBlock(std::move(elseBlock)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "IfStatement\n" +
                            condition->toString(indentLevel + 1) +
                            block->toString(indentLevel + 1);
        if (elseBlock)
        {
            result += indent + "Else\n" + elseBlock->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
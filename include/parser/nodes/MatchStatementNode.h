#ifndef MATCH_STATEMENT_NODE_H
#define MATCH_STATEMENT_NODE_H

#include "StatementNode.h"
#include "ExpressionNode.h"
#include "MatchCaseNode.h"
#include <memory>
#include <vector>

class MatchStatementNode : public StatementNode
{
private:
    std::unique_ptr<ExpressionNode> expression;
    std::vector<std::unique_ptr<MatchCaseNode>> cases;

public:
    MatchStatementNode(std::unique_ptr<ExpressionNode> expression)
        : expression(std::move(expression)) {}

    void addCase(std::unique_ptr<MatchCaseNode> matchCase)
    {
        cases.push_back(std::move(matchCase));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "MatchStatement\n" +
                             expression->toString(indentLevel + 1);
        for (const auto& matchCase : cases)
        {
            result += matchCase->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
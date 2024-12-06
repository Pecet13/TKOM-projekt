#ifndef MATCH_STATEMENT_NODE_H
#define MATCH_STATEMENT_NODE_H

#include "StatementNode.h"
#include <memory>
#include <vector>

class MatchStatementNode : public Node {
private:
    std::unique_ptr<Node> expression;
    std::vector<std::unique_ptr<Node>> cases;

public:
    MatchStatementNode(std::unique_ptr<Node> expression)
        : expression(std::move(expression)) {}

    void addCase(std::unique_ptr<Node> matchCase) {
        cases.push_back(std::move(matchCase));
    }

    std::string toString(int indentLevel = 0) const override {
        std::string indent(indentLevel, '-');
        std::string result = indent + "MatchStatement\n" +
                             expression->toString(indentLevel + 1) + "\n";
        for (const auto& matchCase : cases) {
            result += matchCase->toString(indentLevel + 1) + "\n";
        }
        return result;
    }

};

#endif
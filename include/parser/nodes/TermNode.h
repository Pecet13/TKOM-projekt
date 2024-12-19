#ifndef TERM_NODE_H
#define TERM_NODE_H

#include "Node.h"
#include <memory>

class TermNode : public Node
{
private:
    bool isPositive;
    std::unique_ptr<Node> content;

public:
    TermNode(bool isPositive, std::unique_ptr<Node> content)
        : isPositive(isPositive), content(std::move(content)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "Term\n" + (isPositive ? "" : indent + "!\n") + content->toString(indentLevel + 1);
    }
};

#endif
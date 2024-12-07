#ifndef TERM_NODE_H
#define TERM_NODE_H

#include "Node.h"
#include <memory>

class TermNode : public Node
{
private:
    std::unique_ptr<Node> content;

public:
    TermNode(std::unique_ptr<Node> content)
        : content(std::move(content)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "TermNode\n" + content->toString(indentLevel + 1);
    }
};

#endif
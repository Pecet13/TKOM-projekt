#ifndef TERM_NODE_H
#define TERM_NODE_H

#include "Node.h"
#include <memory>

enum class NegationType
{
    NONE,
    LOGICAL,
    ARITHMETICAL
};

class TermNode : public Node
{
private:
    NegationType negation;
    std::unique_ptr<Node> content;

public:
    TermNode(NegationType negationType, std::unique_ptr<Node> content)
        : negation(negationType), content(std::move(content)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Term";
        if (negation == NegationType::LOGICAL)
        {
            result += "(negation: logical)"; 
        }
        else if (negation == NegationType::ARITHMETICAL)
        {
            result += "(negation: arithmetical)"; 
        }
        return result + "\n" + content->toString(indentLevel + 1);
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    NegationType getNegation() const
    {
        return negation;
    }

    const Node* getContent() const
    {
        return content.get();
    }
};

#endif
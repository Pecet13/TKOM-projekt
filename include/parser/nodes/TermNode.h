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
    NegationType negationType;
    std::unique_ptr<Node> content;

public:
    TermNode(NegationType negationType, std::unique_ptr<Node> content)
        : negationType(negationType), content(std::move(content)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Term\n";
        if (negationType == NegationType::LOGICAL)
        {
            result += "(negation: logical)"; 
        }
        else if (negationType == NegationType::ARITHMETICAL)
        {
            result += "(negation: arithmetical)"; 
        }
        return result + content->toString(indentLevel + 1);
    }
};

#endif
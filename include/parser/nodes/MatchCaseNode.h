#ifndef MATCH_CASE_NODE_H
#define MATCH_CASE_NODE_H

#include "Node.h"
#include <string>
#include <memory>

class MatchCaseNode : public Node
{
private:
    std::string type;
    std::string identifier;
    std::unique_ptr<Node> block;

public:
    MatchCaseNode(const std::string& type, const std::string& identifier, std::unique_ptr<Node> block)
        : type(type), identifier(identifier), block(std::move(block)) {}

    MatchCaseNode(std::unique_ptr<Node> block)
        : type("default"), block(std::move(block)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "MatchCaseNode(\n";
        if (type == "default")
        {
            result += indent + "default)\n";
        }
        else
        {
            result += indent + "type: " + type + "\n" + indent + ", identifier: " + identifier + ")\n";
        }
        result += block->toString(indentLevel + 1);
        return result;
    }
};

#endif
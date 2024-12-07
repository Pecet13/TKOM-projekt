#ifndef FIELD_ACCESS_NODE_H
#define FIELD_ACCESS_NODE_N

#include "Node.h"
#include <string>
#include <vector>

class FieldAccessNode : public Node
{
private:
    std::string baseIdentifier;
    std::vector<std::string> fieldChain;

public:
    FieldAccessNode(std::string base, std::vector<std::string> chain)
        : baseIdentifier(std::move(base)), fieldChain(std::move(chain)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "FieldAccessNode(base: " + baseIdentifier + ")\n";
        indentLevel++;
        for (const auto& field : fieldChain)
        {
            std::string fieldIndent(indentLevel, '-');
            result += fieldIndent + field + "\n";
            indentLevel++;
        }
        return result;
    }
};

#endif
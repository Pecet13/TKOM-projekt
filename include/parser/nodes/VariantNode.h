#ifndef VARIANT_NODE_H
#define VARIANT_NODE_H

#include "Node.h"
#include <string>
#include <vector>

class VariantNode : public Node
{
private:
    std::vector<std::string> types;

public:
    void addType(const std::string& type)
    {
        types.push_back(type);
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "VariantNode\n";
        for (const auto& type : types)
        {
            result += indent + "-" + type + "\n";
        }
        return result;
    }
};

#endif
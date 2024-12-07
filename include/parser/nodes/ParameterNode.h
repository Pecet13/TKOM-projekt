#ifndef PARAMETER_NODE_H
#define PARAMETER_NODE_H

#include "Node.h"
#include <string>

class ParameterNode : public Node
{
private:
    std::string type;
    std::string identifier;

public:
    ParameterNode(const std::string& type, const std::string& identifier)
        : type(type), identifier(identifier) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "ParameterNode(type: " + type + ", identifier: " + identifier + ")\n";
    }
};

#endif
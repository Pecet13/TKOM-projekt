#ifndef PARAMETER_NODE_H
#define PARAMETER_NODE_H

#include "VariantNode.h"
#include <variant>
#include <string>

class ParameterNode : public Node
{
private:
    std::variant<std::string, std::unique_ptr<VariantNode>> type;
    std::string identifier;

public:
    ParameterNode(const std::string& type, const std::string& identifier)
        : type(type), identifier(identifier) {}

    ParameterNode(std::unique_ptr<VariantNode> type, const std::string& identifier)
        : type(std::move(type)), identifier(identifier) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Parameter(type: ";
        if (std::holds_alternative<std::string>(type))
        {
            result += std::get<std::string>(type);
        }
        if (std::holds_alternative<std::unique_ptr<VariantNode>>(type))
        {
            result += std::get<std::unique_ptr<VariantNode>>(type)->toString();
        }
        result += ", identifier: " + identifier + ")\n";
        return result;
    }
};

#endif
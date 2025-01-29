#ifndef VARIANT_NODE_H
#define VARIANT_NODE_H

#include "Node.h"
#include <string>
#include <vector>
#include <variant>
#include <memory>

class VariantNode : public Node
{
private:
    std::vector<std::variant<std::string, std::unique_ptr<VariantNode>>> types;

public:
    void addType(const std::string& type)
    {
        types.push_back(type);
    }

    void addType(std::unique_ptr<VariantNode> type)
    {
        types.push_back(std::move(type));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Variant\n";
        for (const auto& type : types)
        {
            if (std::holds_alternative<std::string>(type))
            {
                result += indent + "-" + std::get<std::string>(type) + "\n";
            }
            else
            {
                result += std::get<std::unique_ptr<VariantNode>>(type)->toString(indentLevel + 1);
            }
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    const std::vector<std::variant<std::string, std::unique_ptr<VariantNode>>>& getTypes() const
    {
        return types;
    }
};

#endif
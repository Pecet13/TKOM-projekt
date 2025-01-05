#ifndef STRUCT_FIELD_NODE_H
#define STRUCT_FIELD_NODE_H

#include "VariantDeclarationNode.h"
#include <string>
#include <variant>
#include <memory>

class StructFieldNode : public Node
{
private:
    bool isMutable;
    std::variant<std::pair<std::string, std::string>, std::unique_ptr<VariantDeclarationNode>> content;

public:
    StructFieldNode(const std::string& type, const std::string& identifier, bool isMutable = false)
        : isMutable(isMutable), content(std::make_pair(type, identifier)) {}

    StructFieldNode(std::unique_ptr<VariantDeclarationNode> variantDeclaration)
        : isMutable(false), content(std::move(variantDeclaration)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        if (std::holds_alternative<std::pair<std::string, std::string>>(content))
        {
            return indent + "StructField(" + (isMutable ? "mut, " : "") +
                "type: " + std::get<std::pair<std::string, std::string>>(content).first +
                ", identifier: " + std::get<std::pair<std::string, std::string>>(content).second + ")\n";
        }
        else if (std::holds_alternative<std::unique_ptr<VariantDeclarationNode>>(content))
        {
            return indent + "StructField\n" + std::get<std::unique_ptr<VariantDeclarationNode>>(content)->toString(indentLevel + 1);
        }
        return indent + "StructField\n";
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

#endif
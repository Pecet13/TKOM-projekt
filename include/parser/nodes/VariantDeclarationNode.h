#ifndef VARIANT_DECLARATION_NODE_H
#define VARIANT_DECLARATION_NODE_H

#include "DeclarationNode.h"
#include "VariantNode.h"
#include <string>
#include <vector>
#include <memory>

class VariantDeclarationNode : public DeclarationNode
{
private:
    std::string identifier;
    std::unique_ptr<VariantNode> variant;

public:
    VariantDeclarationNode(const std::string& identifier, std::unique_ptr<VariantNode> variant)
        : identifier(identifier), variant(std::move(variant)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "VariantDeclaration(identifier: " + identifier + ")\n"+ variant->toString(indentLevel + 1);
    }
};

#endif
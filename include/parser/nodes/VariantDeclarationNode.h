#ifndef VARIANT_DECLARATION_NODE_H
#define VARIANT_DECLARATION_NODE_H

#include "DeclarationNode.h"
#include "StatementNode.h"
#include "VariantNode.h"
#include <string>
#include <vector>
#include <memory>

class VariantDeclarationNode : public DeclarationNode, public StatementNode
{
private:
    std::unique_ptr<VariantNode> variant;
    std::string identifier;

public:
    VariantDeclarationNode(std::unique_ptr<VariantNode> variant, const std::string& identifier)
        : variant(std::move(variant)), identifier(identifier)  {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        return indent + "VariantDeclaration(identifier: " + identifier + ")\n"+ variant->toString(indentLevel + 1);
    }
};

#endif
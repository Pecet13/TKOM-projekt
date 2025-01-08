#ifndef MATCH_CASE_NODE_H
#define MATCH_CASE_NODE_H

#include "BlockNode.h"
#include "VariantNode.h"
#include <variant>
#include <string>
#include <memory>

class MatchCaseNode : public Node
{
private:
    std::variant<std::string, std::unique_ptr<VariantNode>> type;
    std::string identifier;
    std::unique_ptr<BlockNode> block;

public:
    MatchCaseNode(const std::string& type, const std::string& identifier, std::unique_ptr<BlockNode> block)
        : type(type), identifier(identifier), block(std::move(block)) {}

    MatchCaseNode(std::unique_ptr<VariantNode> type, const std::string& identifier, std::unique_ptr<BlockNode> block)
        : type(std::move(type)), identifier(identifier), block(std::move(block)) {}

    MatchCaseNode(std::unique_ptr<BlockNode> block)
        : type("default"), block(std::move(block)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "MatchCase";
        if (std::holds_alternative<std::string>(type))
        {
            if (std::get<std::string>(type) == "default")
            {
                result += "(default)\n";
            }
            else
            {
                result += "(type: " + std::get<std::string>(type) + ", identifier: " + identifier + ")\n";
            }
        }
        else
        {
            result += std::get<std::unique_ptr<VariantNode>>(type)->toString(indentLevel + 1);
        }
        result += block->toString(indentLevel + 1);
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    const std::variant<std::string, std::unique_ptr<VariantNode>>& getType() const
    {
        return type;
    }

    std::string getIdentifier() const
    {
        return identifier;
    }

    const BlockNode* getBlock() const
    {
        return block.get();
    }
};

#endif
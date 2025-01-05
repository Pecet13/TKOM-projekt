#ifndef FIELD_OR_FUN_CALL_NODE_H
#define FIELD_OR_FUN_CALL_NODE_H

#include "ArgumentListNode.h"
#include <string>
#include <vector>
#include <memory>
#include <variant>

class FieldOrFunCallNode : public Node
{
private:
    std::string identifier;
    std::variant<std::monostate, std::vector<std::string>, std::unique_ptr<ArgumentListNode>> additionalContent;

public:
    FieldOrFunCallNode(const std::string& identifier)
        : identifier(identifier), additionalContent(std::monostate{}) {}

    FieldOrFunCallNode(const std::string& identifier, const std::vector<std::string>& fields)
        : identifier(identifier), additionalContent(fields) {}

    FieldOrFunCallNode(const std::string& identifier, std::unique_ptr<ArgumentListNode> arguments)
        : identifier(identifier), additionalContent(std::move(arguments)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "FieldOrFunCall(identifier: " + identifier + ")\n";
        if (std::holds_alternative<std::vector<std::string>>(additionalContent))
        {
            indentLevel++;
            const auto& fields = std::get<std::vector<std::string>>(additionalContent);
            for (const auto& field : fields)
            {
                std::string fieldIndent(indentLevel, '-');
                result += fieldIndent + "(identifier: " + field + ")\n";
                indentLevel++;
            }
        }
        else if (std::holds_alternative<std::unique_ptr<ArgumentListNode>>(additionalContent))
        {
            const auto& arguments = std::get<std::unique_ptr<ArgumentListNode>>(additionalContent);
            result += arguments->toString(indentLevel + 1);
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

#endif
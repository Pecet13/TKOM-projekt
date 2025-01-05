#ifndef STRUCT_FIELD_LIST_NODE_H
#define STRUCT_FIELD_LIST_NODE_H

#include "StructFieldNode.h"
#include <vector>
#include <memory>

class StructFieldListNode : public Node
{
private:
    std::vector<std::unique_ptr<StructFieldNode>> fields;

public:
    void addField(std::unique_ptr<StructFieldNode> field)
    {
        fields.push_back(std::move(field));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "StructFieldList\n";
        for (const auto& field : fields)
        {
            result += field->toString(indentLevel + 1);
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

#endif
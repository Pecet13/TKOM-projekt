#ifndef STRUCT_FIELD_LIST_NODE_H
#define STRUCT_FIELD_LIST_NODE_H

#include "Node.h"
#include <vector>
#include <memory>

class StructFieldListNode : public Node
{
private:
    std::vector<std::unique_ptr<Node>> fields;

public:
    StructFieldListNode(std::vector<std::unique_ptr<Node>> fields)
        : fields(std::move(fields)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "StructFieldListNode\n";
        for (const auto& field : fields)
        {
            result += field->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
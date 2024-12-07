#ifndef ID_OR_FUNCTION_CALL_NODE_H
#define ID_OR_FUNCTION_CALL_NODE_H

#include "Node.h"
#include <string>
#include <memory>

class IdOrFunctionCallNode : public Node
{
private:
    std::string identifier;
    std::unique_ptr<Node> argument_list;

public:
    IdOrFunctionCallNode(const std::string& identifier, std::unique_ptr<Node> argument_list = nullptr)
        : identifier(identifier), argument_list(std::move(argument_list)) {}
    
    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "IdOrFunctionCallNode(identifier: " + identifier + ")\n";
        if (argument_list)
        {
            result += argument_list->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
#ifndef PARAMETER_LIST_NODE_H
#define PARAMETER_LIST_NODE_H

#include "ParameterNode.h"
#include <vector>
#include <memory>

class ParameterListNode : public Node
{
private:
    std::vector<std::unique_ptr<ParameterNode>> parameters;

public:
    void addParameter(std::unique_ptr<ParameterNode> parameter)
    {
        parameters.push_back(std::move(parameter));
    }

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "ParameterList\n";
        for (const auto& param : parameters)
        {
            result += param->toString(indentLevel + 1);
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

#endif
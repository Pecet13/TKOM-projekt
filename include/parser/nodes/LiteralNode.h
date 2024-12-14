#ifndef LITERAL_NODE_H
#define LITERAL_NODE_H

#include "Node.h"
#include <variant>
#include <string>

class LiteralNode : public Node
{
private:
    std::variant<int, float, std::string, bool> value;

public:
    LiteralNode(int value)
        : value(value) {}

    LiteralNode(float value)
        : value(value) {}

    LiteralNode(const std::string& value)
        : value(value) {}

    LiteralNode(bool value)
        : value(value) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "Literal(";
        if (std::holds_alternative<int>(value))
        {
            result += "int: " + std::to_string(std::get<int>(value));
        }
        else if (std::holds_alternative<float>(value))
        {
            result += "float: " + std::to_string(std::get<float>(value));
        }
        else if (std::holds_alternative<std::string>(value))
        {
            result += "string: \"" + std::get<std::string>(value) + "\"";
        }
        else if (std::holds_alternative<bool>(value))
        {
            result += "bool: " + std::string(std::get<bool>(value) ? "true" : "false");
        }
        result += ")\n";
        return result;
    }
};

#endif
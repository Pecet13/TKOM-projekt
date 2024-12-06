#ifndef VARIABLE_DECLARATION_NODE_H
#define VARIABLE_DECLARATION_NODE_H

#include "DeclarationNode.h"
#include <string>
#include <memory>

class VariableDeclarationNode : public DeclarationNode
{
private:
    std::string type;
    std::string name;
    std::unique_ptr<Node> initializer;

public:
    VariableDeclarationNode(const std::string& type, const std::string& name, std::unique_ptr<Node> initializer)
        : type(type), name(name), initializer(std::move(initializer)) {}

    std::string toString(int indentLevel = 0) const override {
        std::string indent(indentLevel, '-');
        std::string result = indent + "VariableDeclaration(type: " + type + ", name: " + name + ")";
        if (initializer) {
            result += "\n" + initializer->toString(indentLevel + 1);
        }
        return result;
    }
};

#endif
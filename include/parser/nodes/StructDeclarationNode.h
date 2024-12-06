#ifndef STRUCT_DECLARATION_NODE_H
#define STRUCT_DECLARATION_NODE_H

#include "DeclarationNode.h"
#include <string>
#include <vector>
#include <memory>

class StructDeclarationNode : public DeclarationNode {
private:
    std::string name;
    std::vector<std::unique_ptr<Node>> fields;

public:
    StructDeclarationNode(const std::string& name)
        : name(name) {}

    void addField(std::unique_ptr<Node> field) {
        fields.push_back(std::move(field));
    }

    std::string toString(int indentLevel = 0) const override {
        std::string indent(indentLevel, '-');
        std::string result = indent + "StructDeclaration(name: " + name + ")\n";
        for (const auto& field : fields) {
            result += field->toString(indentLevel + 1) + "\n";
        }
        return result;
    }
};

#endif
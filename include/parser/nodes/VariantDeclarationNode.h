#ifndef VARIANT_DECLARATION_NODE_H
#define VARIANT_DECLARATION_NODE_H

#include "Node.h"
#include <string>
#include <vector>
#include <memory>

class VariantDeclarationNode : public Node {
private:
    std::string name;
    std::vector<std::string> types;

public:
    VariantDeclarationNode(const std::string& name)
        : name(name) {}

    void addType(const std::string& type) {
        types.push_back(type);
    }

    std::string toString(int indentLevel = 0) const override {
        std::string indent(indentLevel, '-');
        std::string result = indent + "VariantDeclaration(name: " + name + ")\n";
        for (const auto& type : types) {
            result += indent + "-" + type + "\n";
        }
        return result;
    }
};

#endif
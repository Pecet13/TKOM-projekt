#ifndef VARIABLE_DECLARATION_NODE_H
#define VARIABLE_DECLARATION_NODE_H

#include "DeclarationNode.h"
#include "StatementNode.h"
#include "ExpressionNode.h"
#include <string>
#include <memory>

class VariableDeclarationNode : public DeclarationNode, public StatementNode
{
private:
    bool isMutable;
    std::string type;
    std::string identifier;
    std::unique_ptr<ExpressionNode> expression;

public:
    VariableDeclarationNode(bool isMutable, const std::string& type, 
                            const std::string& identifier, std::unique_ptr<ExpressionNode> expression = nullptr)
        : isMutable(isMutable), type(type), identifier(identifier), expression(std::move(expression)) {}

    std::string toString(int indentLevel = 0) const override
    {
        std::string indent(indentLevel, '-');
        std::string result = indent + "VariableDeclaration(" + (isMutable ? "mut, " : "") + "type: " + type + ", identifier: " + identifier + ")\n";
        if (expression)
        {
            result += expression->toString(indentLevel + 1);
        }
        return result;
    }

    void accept(NodeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

    bool getMutable() const
    {
        return isMutable;
    }

    std::string getType() const
    {
        return type;
    }

    std::string getIdentifier() const
    {
        return identifier;
    }

    const ExpressionNode* getExpression() const
    {
        return expression.get();
    }
};

#endif
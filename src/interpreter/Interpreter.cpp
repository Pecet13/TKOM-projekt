#include "interpreter/Interpreter.h"

void Interpreter::visit(const ProgramNode& node)
{
    for (const auto& decl : node.getDeclarations())
    {
        decl->accept(*this);
    }
}

void Interpreter::visit(const FunctionDeclarationNode& node)
{
    if (functions.find(node.getIdentifier()) != functions.end())
    {
        throw InterpreterException("Duplicate function declaration: " + node.getIdentifier());
    }
    functions[node.getIdentifier()] = &node;

    std::string type = node.getType();

    auto parameters = node.getPararmeters();
    parameters->accept(*this);

    auto block = node.getBlock();
    block->accept(*this);
}

void Interpreter::visit(const VariableDeclarationNode& node)
{
    if (variables.find(node.getIdentifier()) != variables.end())
    {
        throw InterpreterException("Duplicate variable declaration: " + node.getIdentifier());
    }

    bool isMutable = node.getMutable();
    std::string type = node.getType();

    auto expression = node.getExpression();
    expression->accept(*this);
}

void Interpreter::visit(const VariantDeclarationNode& node)
{
    auto variant = node.getVariant();
    variant->accept(*this);

    std::string identifier = node.getIdentifier();
}

void Interpreter::visit(const StructDeclarationNode& node)
{
    std::string identifier = node.getIdentifier();

    auto fields = node.getFields();
    fields->accept(*this);
}

void Interpreter::visit(const BlockNode& node)
{
    for (const auto& stmt : node.getStatements())
    {
        stmt->accept(*this);
    }
}

void Interpreter::visit(const AssignmentNode& node)
{
    auto fieldOrFunCall = node.getFieldOrFunCall();
    fieldOrFunCall->accept(*this);

    auto expression = node.getExpression();
    expression->accept(*this);
}

void Interpreter::visit(const IfStatementNode& node)
{
    auto condition = node.getCondition();
    condition->accept(*this);

    auto block = node.getBlock();
    block->accept(*this);

    auto elseBlock = node.getElseBlock();
    elseBlock->accept(*this);
}

void Interpreter::visit(const WhileStatementNode& node)
{
    auto condition = node.getCondition();
    condition->accept(*this);

    auto block = node.getBlock();
    block->accept(*this);
}

void Interpreter::visit(const ReturnStatementNode& node)
{
    auto expression = node.getExpression();
    expression->accept(*this);
}

void Interpreter::visit(const MatchStatementNode& node)
{
    auto expression = node.getExpression();
    expression->accept(*this);

    for (const auto& matchCase : node.getCases())
    {
        matchCase->accept(*this);
    }
}

void Interpreter::visit(const ParameterListNode& node)
{
    for (const auto& param : node.getParameters())
    {
        param->accept(*this);
    }
}

void Interpreter::visit(const ParameterNode& node)
{
    if (std::holds_alternative<std::string>(node.getType()))
    {
        std::string type = std::get<std::string>(node.getType());
    }
    else if (std::holds_alternative<std::unique_ptr<VariantNode>>(node.getType()))
    {
        auto type = std::get<std::unique_ptr<VariantNode>>(node.getType()).get();
        type->accept(*this);
    }

    std::string identifier = node.getIdentifier();
}

void Interpreter::visit(const ExpressionNode& node)
{
    auto left = node.getLeft();
    left->accept(*this);

    for (const auto& right : node.getRights())
    {
        right->accept(*this);
    }
}

void Interpreter::visit(const AndExpressionNode& node)
{
    auto left = node.getLeft();
    left->accept(*this);

    for (const auto& right : node.getRights())
    {
        right->accept(*this);
    }
}

void Interpreter::visit(const ComparisonNode& node)
{
    auto left = node.getLeft();
    left->accept(*this);

    ComparisonOperator op = node.getOp();

    auto right = node.getRight();
    right->accept(*this);
}

void Interpreter::visit(const AddExpressionNode& node)
{
    auto left = node.getLeft();
    left->accept(*this);

    for (const auto& pair : node.getRights())
    {
        AddOperator op = pair.first;

        auto right = pair.second.get();
        right->accept(*this);
    }
}

void Interpreter::visit(const MultExpressionNode& node)
{
    auto left = node.getLeft();
    left->accept(*this);

    for (const auto& pair : node.getRights())
    {
        MultOperator op = pair.first;

        auto right = pair.second.get();
        right->accept(*this);
    }
}

void Interpreter::visit(const TermNode& node)
{
    NegationType negation = node.getNegation();

    auto content = node.getContent();
    content->accept(*this);
}

void Interpreter::visit(const FieldOrFunCallNode& node)
{
    std::string identifier = node.getIdentifier();

    if (std::holds_alternative<std::vector<std::string>>(node.getAdditionalContent()))
    {
        auto fields = std::get<std::vector<std::string>>(node.getAdditionalContent());
    }
    else if (std::holds_alternative<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()))
    {
        auto arguments = std::get<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()).get();
        arguments->accept(*this);
    }
}

void Interpreter::visit(const ArgumentListNode& node)
{
    for (const auto& arg : node.getArguments())
    {
        arg->accept(*this);
    }
}

void Interpreter::visit(const StructCreationNode& node)
{
    std::string type = node.getType();
    std::string identifier = node.getIdentifier();

    auto arguments = node.getArguments();
    arguments->accept(*this);
}

void Interpreter::visit(const StructFieldListNode& node)
{
    for (const auto& field : node.getFields())
    {
        field->accept(*this);
    }
}

void Interpreter::visit(const StructFieldNode& node)
{
    bool isMutable = node.getMutable();

    if (std::holds_alternative<std::pair<std::string, std::string>>(node.getContent()))
    {
        std::string type = std::get<std::pair<std::string, std::string>>(node.getContent()).first;
        std::string identifier = std::get<std::pair<std::string, std::string>>(node.getContent()).second;
    }
    else if (std::holds_alternative<std::unique_ptr<VariantDeclarationNode>>(node.getContent()))
    {
        auto variantDeclaration = std::get<std::unique_ptr<VariantDeclarationNode>>(node.getContent()).get();
        variantDeclaration->accept(*this);
    }
}

void Interpreter::visit(const VariantNode& node)
{
    for (const auto& type : node.getTypes())
    {
        if (std::holds_alternative<std::string>(type))
        {
            std::string regularType = std::get<std::string>(type);
        }
        else if (std::holds_alternative<std::unique_ptr<VariantNode>>(type))
        {
            auto variant = std::get<std::unique_ptr<VariantNode>>(type).get();
            variant->accept(*this);
        }
    }
}

void Interpreter::visit(const MatchCaseNode& node)
{
    if (std::holds_alternative<std::string>(node.getType()))
    {
        std::string regularType = std::get<std::string>(node.getType());
    }
    else if (std::holds_alternative<std::unique_ptr<VariantNode>>(node.getType()))
    {
        auto variant = std::get<std::unique_ptr<VariantNode>>(node.getType()).get();
        variant->accept(*this);
    }

    std::string identifier = node.getIdentifier();

    auto block = node.getBlock();
    block->accept(*this);
}

void visit(const IntLiteralNode& node)
{
    int value = node.getValue();
}

void visit(const FloatLiteralNode& node)
{
    float value = node.getValue();
}

void visit(const BoolLiteralNode& node)
{
    bool value = node.getValue();
}

void visit(const StringLiteralNode& node)
{
    std::string value = node.getValue();
}
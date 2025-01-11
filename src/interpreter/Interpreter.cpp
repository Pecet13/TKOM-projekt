#include "interpreter/Interpreter.h"

Scope& Interpreter::currentScope()
{
    return scopes.back();
}

void Interpreter::enterScope()
{
    scopes.emplace_back();
}

void Interpreter::exitScope()
{
    if (scopes.size() <= 1)
    {
        throw InterpreterException("Cannot exit global scope");
    }
    scopes.pop_back();
}

std::variant<int, float, std::string, bool> Interpreter::castValueType(const std::variant<int, float, std::string, bool>& value, const std::string& targetType)
{
    if (targetType == "int")
    {
        if (std::holds_alternative<float>(value))
        {
            return static_cast<int>(std::get<float>(value));
        }
        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value) ? 1 : 0;
        }
        if (std::holds_alternative<std::string>(value))
        {
            try
            {
                return std::stoi(std::get<std::string>(value));
            }
            catch (const std::invalid_argument& e)
            {
                throw InterpreterException("Cannot convert to integer (invalid argument): " + std::get<std::string>(value));
            }
            catch (const std::out_of_range& e)
            {
                throw InterpreterException("Cannot convert to integer (overflow): " + std::get<std::string>(value));
            }
        }
    }
    if (targetType == "float")
    {
        if (std::holds_alternative<int>(value))
        {
            return static_cast<float>(std::get<int>(value));
        }
        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value) ? 1.0f : 0.0f;
        }
        if (std::holds_alternative<std::string>(value))
        {
            try
            {
                return std::stof(std::get<std::string>(value));
            }
            catch (const std::invalid_argument& e)
            {
                throw InterpreterException("Cannot convert to float (invalid argument): " + std::get<std::string>(value));
            }
            catch (const std::out_of_range& e)
            {
                throw InterpreterException("Cannot convert to float (overflow): " + std::get<std::string>(value));
            }
        }
    }
    if (targetType == "string")
    {
        if (std::holds_alternative<int>(value))
        {
            return std::to_string(std::get<int>(value));
        }
        if (std::holds_alternative<float>(value))
        {
            return std::to_string(std::get<float>(value));
        }
        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value) ? "true" : "false";
        }
    }
    if (targetType == "bool")
    {
        if (std::holds_alternative<int>(value))
        {
            return std::get<int>(value) != 0;
        }
        if (std::holds_alternative<float>(value))
        {
            return std::get<float>(value) != 0.0f;
        }
        if (std::holds_alternative<std::string>(value))
        {
            return (std::get<std::string>(value) != "false" 
                    && std::get<std::string>(value) != "0"
                    && std::get<std::string>(value) != "");
        }
    }

    return value;
}

Variable& Interpreter::getVariable(const std::string& identifier)
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        if (it->variables.find(identifier) != it->variables.end())
        {
            return it->variables.at(identifier);
        }
    }
    throw InterpreterException("Variable not found: " + identifier);
}

void Interpreter::updateVariable(const std::string& identifier, const std::variant<int, float, std::string, bool>& newValue)
{
    Variable& variable = getVariable(identifier);
    
    if (!variable.isMutable)
    {
        throw InterpreterException("Cannot modify a const variable: " + identifier);
    }

    if (variable.value.index() != newValue.index())
    {
        throw InterpreterException("Incorrect value type for variable: " + identifier);
    }
    variable.value = newValue;
}

Interpreter::Interpreter()
{
    scopes.emplace_back();
}

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
}

void Interpreter::visit(const VariableDeclarationNode& node)
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        if (it->variables.find(node.getIdentifier()) != it->variables.end())
        {
            throw InterpreterException("Duplicate variable declaration: " + node.getIdentifier());
        }
    }
    std::variant<int, float, std::string, bool> value;

    if (node.getExpression() != nullptr)
    {
        node.getExpression()->accept(*this);
        value = valueStack.top();
        valueStack.pop();
    }

    value = castValueType(value, node.getType());

    currentScope().variables[node.getIdentifier()] = Variable{node.getMutable(), node.getType(), value};
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

        auto block = functions[identifier]->getBlock();
        block->accept(*this);
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

void Interpreter::visit(const IntLiteralNode& node)
{
    valueStack.push(node.getValue());
}

void Interpreter::visit(const FloatLiteralNode& node)
{
    valueStack.push(node.getValue());
}

void Interpreter::visit(const BoolLiteralNode& node)
{
    valueStack.push(node.getValue());
}

void Interpreter::visit(const StringLiteralNode& node)
{
    valueStack.push(node.getValue());
}
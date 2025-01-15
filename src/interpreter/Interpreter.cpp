#include "interpreter/Interpreter.h"

Scope& Interpreter::currentScope()
{
    if (!callStack.empty())
    {
        return callStack.top().scopes.back();
    }
    return globalScope;
}

void Interpreter::enterScope()
{
    callStack.top().scopes.emplace_back();
}

void Interpreter::exitScope()
{
    callStack.top().scopes.pop_back();
}

void Interpreter::enterCallContext(const std::string& identifier)
{
    callStack.push(FunctionCallContext{identifier});
    enterScope();
}

void Interpreter::exitCallContext()
{
    callStack.pop();
}

void Interpreter::checkDuplicateId(const std::string& identifier)
{
    if (!callStack.empty())
    {
        for (auto it = callStack.top().scopes.rbegin(); it != callStack.top().scopes.rend(); ++it)
        {
            if (it->variables.find(identifier) != it->variables.end() ||
                it->structs.find(identifier) != it->structs.end() ||
                it->structInstances.find(identifier) != it->structInstances.end() ||
                it->variants.find(identifier) != it->variants.end())
            {
                throw InterpreterException("Identifier already in use: " + identifier);
            }
        }
    }
    if (globalScope.variables.find(identifier) != globalScope.variables.end() ||
        globalScope.structs.find(identifier) != globalScope.structs.end() ||
        globalScope.structInstances.find(identifier) != globalScope.structInstances.end() ||
        globalScope.variants.find(identifier) != globalScope.variants.end())
    {
        throw InterpreterException("Identifier already in use: " + identifier);
    }
}

Value Interpreter::castValueType(const Value& value, const std::string& targetType)
{
    if (std::holds_alternative<std::unique_ptr<Variant>>(value))
    {
        throw InterpreterException("Cannot cast value from type: variant");
    }
    if (std::holds_alternative<std::unique_ptr<StructureInstance>>(value))
    {
        throw InterpreterException("Cannot cast value from type: " + std::get<std::unique_ptr<StructureInstance>>(value).get()->type);
    }
    if (targetType == "int")
    {
        if (std::holds_alternative<int>(value))
        {
            return std::get<int>(value);
        }
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
        if (std::holds_alternative<float>(value))
        {
            return std::get<float>(value);
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
        else
        {
            throw InterpreterException("Cannot cast value from this type");
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
        if (std::holds_alternative<std::string>(value))
        {
            return std::get<std::string>(value);
        }
        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value) ? "true" : "false";
        }
        else
        {
            throw InterpreterException("Cannot cast value from this type");
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
            return (std::get<std::string>(value) != "false" &&
                    std::get<std::string>(value) != "0" &&
                    std::get<std::string>(value) != "");
        }
        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value);
        }
        else
        {
            throw InterpreterException("Cannot cast value from this type");
        }
    }

    throw InterpreterException("Unsupported type for converting value: " + targetType);
}

Variable& Interpreter::getVariable(const std::string& identifier)
{
    for (auto it = callStack.top().scopes.rbegin(); it != callStack.top().scopes.rend(); ++it)
    {
        if (it->variables.find(identifier) != it->variables.end())
        {
            return it->variables.at(identifier);
        }
    }
    if (globalScope.variables.find(identifier) != globalScope.variables.end())
    {
        return globalScope.variables.at(identifier);
    }
    throw InterpreterException("Variable not found: " + identifier);
}

Structure& Interpreter::getStructure(const std::string& identifier)
{
    for (auto it = callStack.top().scopes.rbegin(); it != callStack.top().scopes.rend(); ++it)
    {
        if (it->structs.find(identifier) != it->structs.end())
        {
            return it->structs.at(identifier);
        }
    }
    if (globalScope.structs.find(identifier) != globalScope.structs.end())
    {
        return globalScope.structs.at(identifier);
    }
    throw InterpreterException("Struct not found: " + identifier);
}

Variant& Interpreter::getVariant(const std::string& identifier)
{
    for (auto it = callStack.top().scopes.rbegin(); it != callStack.top().scopes.rend(); ++it)
    {
        if (it->variants.find(identifier) != it->variants.end())
        {
            return it->variants.at(identifier);
        }
    }
    if (globalScope.variants.find(identifier) != globalScope.variants.end())
    {
        return globalScope.variants.at(identifier);
    }
    throw InterpreterException("Variant not found: " + identifier);
}

StructureInstance& Interpreter::getStructureInstance(const std::string& identifier)
{
    for (auto it = callStack.top().scopes.rbegin(); it != callStack.top().scopes.rend(); ++it)
    {
        if (it->structInstances.find(identifier) != it->structInstances.end())
        {
            return it->structInstances.at(identifier);
        }
    }
    if (globalScope.structInstances.find(identifier) != globalScope.structInstances.end())
    {
        return globalScope.structInstances.at(identifier);
    }
    throw InterpreterException("Struct instance not found: " + identifier);
}

std::string Interpreter::determineValueType(const Value& value)
{
    if (std::holds_alternative<int>(value))
    {
        return "int";
    }
    if (std::holds_alternative<float>(value))
    {
        return "float";
    }
    if (std::holds_alternative<std::string>(value))
    {
        return "string";
    }
    if (std::holds_alternative<bool>(value))
    {
        return "bool";
    }
    if (std::holds_alternative<std::unique_ptr<Variant>>(value))
    {
        return "variant";
    }
    if (std::holds_alternative<std::unique_ptr<StructureInstance>>(value))
    {
        return std::get<std::unique_ptr<StructureInstance>>(value).get()->type;
    }
    throw InterpreterException("Invalid value type");
}

Interpreter::Interpreter()
{}

void Interpreter::visit(const ProgramNode& node)
{
    for (const auto& decl : node.getDeclarations())
    {
        decl->accept(*this);
    }
    if (functions.find("main") == functions.end())
    {
        throw InterpreterException("Missing main function");
    }
    enterCallContext("main");
    functions["main"].block->accept(*this);
}

void Interpreter::visit(const FunctionDeclarationNode& node)
{
    if (functions.find(node.getIdentifier()) != functions.end())
    {
        throw InterpreterException("Duplicate function declaration: " + node.getIdentifier());
    }
    if (node.getIdentifier() == "main" &&  node.getType() != "int")
    {
        throw InterpreterException("main must return int");
    }

    node.getPararmeters()->accept(*this);

    std::vector<std::pair<std::string, std::string>> parameters;

    for (auto i = 0; i < parameterIdentifiers.size(); ++i)
    {
        parameters.push_back(std::make_pair(parameterTypes[i], parameterIdentifiers[i]));
    }

    functions[node.getIdentifier()] = Function{node.getType(), parameters, node.getBlock()};
    parameterTypes.clear();
    parameterIdentifiers.clear();
}

void Interpreter::visit(const VariableDeclarationNode& node)
{
    checkDuplicateId(node.getIdentifier());
    Value value;

    if (node.getExpression() != nullptr)
    {
        node.getExpression()->accept(*this);
        value = std::move(valueStack.top());
        valueStack.pop();
    }

    value = castValueType(value, node.getType());

    currentScope().variables[node.getIdentifier()] = Variable{node.getMutable(), node.getType(), std::move(value)};
}

void Interpreter::visit(const VariantDeclarationNode& node)
{
    checkDuplicateId(node.getIdentifier());

    node.getVariant()->accept(*this);

    currentScope().variants[node.getIdentifier()] = Variant{parameterTypes.back()};
    parameterTypes.pop_back();
}

void Interpreter::visit(const StructDeclarationNode& node)
{
    checkDuplicateId(node.getIdentifier());

    node.getFields()->accept(*this);

    currentScope().structs[node.getIdentifier()] = Structure{fieldBuffer};
    fieldBuffer.clear();
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
    node.getFieldOrFunCall()->accept(*this);

    if (auto expression = node.getExpression())
    {
        expression->accept(*this);
        // ...
    }
}

void Interpreter::visit(const IfStatementNode& node)
{
    node.getCondition()->accept(*this);

    bool condition = std::get<bool>(castValueType(valueStack.top(), "bool"));
    valueStack.pop();

    if (condition)
    {
        node.getBlock()->accept(*this);
    }
    else
    {
        node.getElseBlock()->accept(*this);
    }
}

void Interpreter::visit(const WhileStatementNode& node)
{
    node.getCondition()->accept(*this);

    bool condition = std::get<bool>(castValueType(valueStack.top(), "bool"));
    valueStack.pop();

    while (condition)
    {
        node.getBlock()->accept(*this);
        node.getCondition()->accept(*this);

        condition = std::get<bool>(castValueType(valueStack.top(), "bool"));
        valueStack.pop();
    }
}

void Interpreter::visit(const ReturnStatementNode& node)
{
    std::string functionIdentifier = callStack.top().identifier;
    if (functions[functionIdentifier].type == "void" && node.getExpression() != nullptr)
    {
        throw InterpreterException("Cannot return a value in void function");
    }
    
    node.getExpression()->accept(*this);

    auto value = std::move(valueStack.top());
    valueStack.pop();
    value = castValueType(value, functions[functionIdentifier].type);
    valueStack.push(std::move(value));

    exitCallContext();
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
        parameterTypes.push_back(std::get<std::string>(node.getType()));
        parameterIdentifiers.push_back(node.getIdentifier());
    }
    else if (std::holds_alternative<std::unique_ptr<VariantNode>>(node.getType()))
    {
        parameterIdentifiers.push_back(node.getIdentifier());
        std::get<std::unique_ptr<VariantNode>>(node.getType()).get()->accept(*this);
    }
}

void Interpreter::visit(const ExpressionNode& node)
{
    node.getLeft()->accept(*this);

    auto a = std::move(valueStack.top());
    valueStack.pop();

    for (const auto& right : node.getRights())
    {
        if (std::get<bool>(castValueType(a, "bool")) == true)
        {
            break;
        }
        right->accept(*this);

        auto b = std::move(valueStack.top());
        valueStack.pop();

        a = std::get<bool>(castValueType(a, "bool")) || std::get<bool>(castValueType(b, "bool"));
    }
    valueStack.push(std::move(a));
}

void Interpreter::visit(const AndExpressionNode& node)
{
    node.getLeft()->accept(*this);

    auto a = std::move(valueStack.top());
    valueStack.pop();

    for (const auto& right : node.getRights())
    {
        if (std::get<bool>(castValueType(a, "bool")) == false)
        {
            break;
        }
        right->accept(*this);

        auto b = std::move(valueStack.top());
        valueStack.pop();

        a = std::get<bool>(castValueType(a, "bool")) && std::get<bool>(castValueType(b, "bool"));
    }
    valueStack.push(std::move(a));
}

void Interpreter::visit(const ComparisonNode& node)
{
    node.getLeft()->accept(*this);

    auto a = std::move(valueStack.top());
    valueStack.pop();

    ComparisonOperator op = node.getOp();

    if (op != ComparisonOperator::NONE)
    {
        node.getRight()->accept(*this);

        auto b = std::move(valueStack.top());
        valueStack.pop();

        switch (op)
        {
            case ComparisonOperator::EQUAL:
            {
                if (std::holds_alternative<int>(a))
                {
                    a = std::get<int>(a) == std::get<int>(castValueType(b, "int"));
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) == std::get<float>(castValueType(b, "float"));
                }
                else if (std::holds_alternative<bool>(a))
                {
                    a = std::get<bool>(a) == std::get<bool>(castValueType(b, "bool"));
                }
                else if (std::holds_alternative<std::string>(a))
                {
                    a = std::get<std::string>(a) == std::get<std::string>(castValueType(b, "string"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " == " + determineValueType(b));
                }
                break;
            }
            case ComparisonOperator::NOT_EQUAL:
            {
                if (std::holds_alternative<int>(a))
                {
                    a = std::get<int>(a) != std::get<int>(castValueType(b, "int"));
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) != std::get<float>(castValueType(b, "float"));
                }
                else if (std::holds_alternative<bool>(a))
                {
                    a = std::get<bool>(a) != std::get<bool>(castValueType(b, "bool"));
                }
                else if (std::holds_alternative<std::string>(a))
                {
                    a = std::get<std::string>(a) != std::get<std::string>(castValueType(b, "string"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " != " + determineValueType(b));
                }
                break;
            }
            case ComparisonOperator::GREATER:
            {
                if (std::holds_alternative<int>(a))
                {
                    a = std::get<int>(a) > std::get<int>(castValueType(b, "int"));
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) > std::get<float>(castValueType(b, "float"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " > " + determineValueType(b));
                }
                break;
            }
            case ComparisonOperator::GREATER_EQUAL:
            {
                if (std::holds_alternative<int>(a))
                {
                    a = std::get<int>(a) >= std::get<int>(castValueType(b, "int"));
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) >= std::get<float>(castValueType(b, "float"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " >= " + determineValueType(b));
                }
                break;
            }
            case ComparisonOperator::LESS:
            {
                if (std::holds_alternative<int>(a))
                {
                    a = std::get<int>(a) < std::get<int>(castValueType(b, "int"));
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) < std::get<float>(castValueType(b, "float"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " < " + determineValueType(b));
                }
                break;
            }
            case ComparisonOperator::LESS_EQUAL:
            {
                if (std::holds_alternative<int>(a))
                {
                    a = std::get<int>(a) <= std::get<int>(castValueType(b, "int"));
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) <= std::get<float>(castValueType(b, "float"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " <= " + determineValueType(b));
                }
                break;
            }
        }
    }
    valueStack.push(std::move(a));
}

void Interpreter::visit(const AddExpressionNode& node)
{
    node.getLeft()->accept(*this);

    auto a = std::move(valueStack.top());
    valueStack.pop();

    for (const auto& pair : node.getRights())
    {
        AddOperator op = pair.first;

        pair.second.get()->accept(*this);

        auto b = std::move(valueStack.top());
        valueStack.pop();

        switch (op)
        {
            case AddOperator::PLUS:
            {
                if (std::holds_alternative<int>(a))
                {
                    a = std::get<int>(a) + std::get<int>(castValueType(b, "int"));
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) + std::get<float>(castValueType(b, "float"));
                }
                else if (std::holds_alternative<std::string>(a))
                {
                    a = std::get<std::string>(a) + std::get<std::string>(castValueType(b, "string"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " + " + determineValueType(b));
                }
                break;
            }
            case AddOperator::MINUS:
            {
                if (std::holds_alternative<int>(a))
                {
                    a = std::get<int>(a) - std::get<int>(castValueType(b, "int"));
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) - std::get<float>(castValueType(b, "float"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " - " + determineValueType(b));
                }
                break;
            }
        }
    }
    valueStack.push(std::move(a));
}

void Interpreter::visit(const MultExpressionNode& node)
{
    node.getLeft()->accept(*this);

    auto a = std::move(valueStack.top());
    valueStack.pop();

    for (const auto& pair : node.getRights())
    {
        MultOperator op = pair.first;

        pair.second.get()->accept(*this);

        auto b = std::move(valueStack.top());
        valueStack.pop();

        switch (op)
        {
            case MultOperator::ASTERISK:
            {
                if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b))
                {
                    a = std::get<int>(a) * std::get<int>(b);
                }
                else if (std::holds_alternative<float>(a) || std::holds_alternative<float>(b))
                {
                    a = std::get<float>(castValueType(a, "float")) * std::get<float>(castValueType(b, "float"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " * " + determineValueType(b));
                }
                break;
            }
            case MultOperator::SLASH:
            {
                if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b))
                {
                    if (std::get<int>(b) == 0)
                    {
                        throw InterpreterException("Division by zero: " + std::get<std::string>(castValueType(a, "string")) +
                                                    " / " + std::get<std::string>(castValueType(b, "string")));
                    }
                    a = std::get<int>(a) / std::get<int>(b);
                }
                else if (std::holds_alternative<float>(a) || std::holds_alternative<float>(b))
                {
                    if (std::get<float>(castValueType(b, "float")) == 0)
                    {
                        throw InterpreterException("Division by zero: " + std::get<std::string>(castValueType(a, "string")) +
                                                    " / " + std::get<std::string>(castValueType(b, "string")));
                    }
                    a = std::get<float>(castValueType(a, "float")) / std::get<float>(castValueType(b, "float"));
                }
                else
                {
                    throw InterpreterException("Invalid types for operation: " + determineValueType(a) + " / " + determineValueType(b));
                }
                break;
            }
        }
    }
    valueStack.push(std::move(a));
}

void Interpreter::visit(const TermNode& node)
{
    NegationType negation = node.getNegation();

    node.getContent()->accept(*this);

    auto value = std::move(valueStack.top());
    valueStack.pop();

    switch (negation)
    {
        case NegationType::LOGICAL:
        {
            if (std::holds_alternative<int>(value))
            {
                value = !std::get<int>(value);
            }
            else if (std::holds_alternative<float>(value))
            {
                value = !std::get<float>(value);
            }
            else if (std::holds_alternative<std::string>(value))
            {
                value = !std::get<bool>(castValueType(std::get<std::string>(value), "bool"));
            }
            else if (std::holds_alternative<bool>(value))
            {
                value = !std::get<bool>(value);
            }
            else
            {
                throw InterpreterException("Unsupported type for this negation type: !" + determineValueType(value));
            }
            break;
        }
        case NegationType::ARITHMETICAL:
        {
            if (std::holds_alternative<int>(value))
            {
                value = -std::get<int>(value);
            }
            else if (std::holds_alternative<float>(value))
            {
                value = -std::get<float>(value);
            }
            else if (std::holds_alternative<bool>(value))
            {
                value = -std::get<bool>(value);
            }
            else
            {
                throw InterpreterException("Unsupported type for this negation type: -" + determineValueType(value));
            }
            break;
        }
    }

    valueStack.push(std::move(value));
}

void Interpreter::visit(const FieldOrFunCallNode& node)
{
    std::string identifier = node.getIdentifier();

    if (identifier == "print")
    {
        if (!std::holds_alternative<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()))
        {
            throw InterpreterException("Calling print requires an argument");
        }

        auto arguments = std::get<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()).get();
        size_t stackSizeBefore = valueStack.size();
        arguments->accept(*this);
        size_t argumentCount = valueStack.size() - stackSizeBefore;

        if (argumentCount != 1)
        {
            throw InterpreterException("Function " + identifier +
                                        " requires 1 argument(s), but " + std::to_string(argumentCount) +
                                        " were given");
        }

        auto value = std::move(valueStack.top());
        valueStack.pop();

        if (std::holds_alternative<int>(value))
        {
            std::cout << std::get<int>(value);
        }
        else if (std::holds_alternative<float>(value))
        {
            std::cout << std::get<float>(value);
        }
        else if (std::holds_alternative<std::string>(value))
        {
            std::cout << std::get<std::string>(value);
        }
        else if (std::holds_alternative<bool>(value))
        {
            std::cout << std::get<bool>(value);
        }
        return;
    }

    if (std::holds_alternative<std::vector<std::string>>(node.getAdditionalContent()))
    {
        auto fields = std::get<std::vector<std::string>>(node.getAdditionalContent());
    }
    else if (std::holds_alternative<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()))
    {
        if (functions.find(identifier) == functions.end())
        {
            throw InterpreterException("Function not found: " + identifier);
        }

        auto arguments = std::get<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()).get();
        size_t stackSizeBefore = valueStack.size();
        arguments->accept(*this);
        size_t argumentCount = valueStack.size() - stackSizeBefore;

        const Function& function = functions[identifier];

        if (argumentCount != function.parameters.size())
        {
            throw InterpreterException("Function " + identifier +
                                        " requires " + std::to_string(function.parameters.size()) +
                                        " argument(s), but " + std::to_string(argumentCount) +
                                        " were given");
        }

        enterCallContext(identifier);
        for (auto i = function.parameters.size() - 1; i >= 0; --i)
        {
            const auto& param = function.parameters[i];
            auto value = std::move(valueStack.top());
            valueStack.pop();

            if (param.first.find("variant") == 0)
            {
                std::string valueType = determineValueType(std::move(value));
                if (param.first.find(valueType) == std::string::npos)
                {
                    throw InterpreterException("Type " + valueType + " is not allowed in " + param.first);
                }
            }
            else
            {
                value = castValueType(value, param.first);
            }
            
            currentScope().variables[param.second] = Variable{false, param.first, std::move(value)};
        }

        function.block->accept(*this);
    }

    Variable& variable = getVariable(identifier);
    valueStack.push(std::move(variable.value));
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

    Structure& structure = getStructure(type);

    // ...
    
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
    std::string type;
    std::string identifier;

    if (std::holds_alternative<std::pair<std::string, std::string>>(node.getContent()))
    {
        type = std::get<std::pair<std::string, std::string>>(node.getContent()).first;
        identifier = std::get<std::pair<std::string, std::string>>(node.getContent()).second;
    }
    else if (std::holds_alternative<std::unique_ptr<VariantDeclarationNode>>(node.getContent()))
    {
        type = "variant";
        auto variantDeclaration = std::get<std::unique_ptr<VariantDeclarationNode>>(node.getContent()).get();
        variantDeclaration->accept(*this);
        // identifier = variantDeclaration->getIdentifier();
        // currentScope().variants.erase(identifier);
    }
    if (isMutable && type != "int" && type != "float" && type != "string" && type != "bool")
    {
        throw InterpreterException("Cannot use 'mut' to this type of field: " + type);
    }
    fieldBuffer[identifier] = Field{isMutable, type};
}

void Interpreter::visit(const VariantNode& node)
{
    std::string variantType = "variant[";

    for (size_t i = 0; i < node.getTypes().size(); ++i)
    {
        const auto& type = node.getTypes()[i];

        if (std::holds_alternative<std::string>(type))
        {
            variantType += std::get<std::string>(type);
        }
        else if (std::holds_alternative<std::unique_ptr<VariantNode>>(type))
        {
            auto variant = std::get<std::unique_ptr<VariantNode>>(type).get();
            variant->accept(*this);

            variantType += parameterTypes.back();
            parameterTypes.pop_back();
        }

        if (i != node.getTypes().size())
        {
            variantType += ", ";
        }
    }
    variantType += "]";
    parameterTypes.push_back(variantType);
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
    valueStack.push(std::move(node.getValue()));
}

void Interpreter::visit(const FloatLiteralNode& node)
{
    valueStack.push(std::move(node.getValue()));
}

void Interpreter::visit(const BoolLiteralNode& node)
{
    valueStack.push(std::move(node.getValue()));
}

void Interpreter::visit(const StringLiteralNode& node)
{
    valueStack.push(std::move(node.getValue()));
}
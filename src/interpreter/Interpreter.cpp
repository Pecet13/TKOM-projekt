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
    if (++recursionDepth > maxRecursionDepth)
    {
        throw InterpreterException("maximum recursion depth exceeded");
    }

    callStack.push(FunctionCallContext{identifier});
    enterScope();
}

void Interpreter::exitCallContext()
{
    std::string functionIdentifier = callStack.top().identifier;
    while (!callStack.top().scopes.empty()) 
    {
        exitScope();
    }
    callStack.pop();
    --recursionDepth;

    if (functions[functionIdentifier].type != "void")
    {
        if (valueStack.empty())
        {
            throw InterpreterException("non-void function '" + functionIdentifier + "' does not return a value");
        }
    }
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
                throw InterpreterException("identifier already in use: " + identifier);
            }
        }
    }
    if (globalScope.variables.find(identifier) != globalScope.variables.end() ||
        globalScope.structs.find(identifier) != globalScope.structs.end() ||
        globalScope.structInstances.find(identifier) != globalScope.structInstances.end() ||
        globalScope.variants.find(identifier) != globalScope.variants.end())
    {
        throw InterpreterException("identifier already in use: " + identifier);
    }
}

Value Interpreter::castValueType(const Value& value, const std::string& targetType)
{
    if (std::holds_alternative<std::shared_ptr<Variant>>(value))
    {
        throw InterpreterException("cannot cast value from type: variant");
    }
    if (std::holds_alternative<std::shared_ptr<StructureInstance>>(value))
    {
        throw InterpreterException("cannot cast value from type: " + std::get<std::shared_ptr<StructureInstance>>(value).get()->type);
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
                throw InterpreterException("cannot convert to integer (invalid argument): " + std::get<std::string>(value));
            }
            catch (const std::out_of_range& e)
            {
                throw InterpreterException("cannot convert to integer (overflow): " + std::get<std::string>(value));
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
                throw InterpreterException("cannot convert to float (invalid argument): " + std::get<std::string>(value));
            }
            catch (const std::out_of_range& e)
            {
                throw InterpreterException("cannot convert to float (overflow): " + std::get<std::string>(value));
            }
        }
        else
        {
            throw InterpreterException("cannot cast value from this type");
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
            std::string str = std::get<bool>(value) ? "true" : "false";
            return str;
        }
        else
        {
            throw InterpreterException("cannot cast value from this type");
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
            throw InterpreterException("cannot cast value from this type");
        }
    }

    throw InterpreterException("unsupported type for converting value: " + targetType);
}

Function& Interpreter::getFunction(const std::string& identifier)
{
    if (functions.find(identifier) == functions.end())
    {
        throw InterpreterException("function not found: " + identifier);
    }

    return functions[identifier];
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
    throw InterpreterException("variable not found: " + identifier);
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
    throw InterpreterException("struct not found: " + identifier);
}

std::shared_ptr<Variant>& Interpreter::getVariant(const std::string& identifier)
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
    throw InterpreterException("variant not found: " + identifier);
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
    throw InterpreterException("struct instance not found: " + identifier);
}

std::string Interpreter::buildVariantType(const Variant& variant)
{
    std::string type = "variant[";
    for (size_t i = 0; i < variant.allowedTypes.size(); ++i)
    {
        if (i > 0)
            type += ", ";
        type += variant.allowedTypes[i];
    }
    type += "]";
    return type;
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
    if (std::holds_alternative<std::shared_ptr<Variant>>(value))
    {
        return buildVariantType(*std::get<std::shared_ptr<Variant>>(value));
    }
    if (std::holds_alternative<std::shared_ptr<StructureInstance>>(value))
    {
        return std::get<std::shared_ptr<StructureInstance>>(value).get()->type;
    }
    throw InterpreterException("invalid value type");
}

Interpreter::Interpreter(const size_t maxRecursionDepth, const size_t maxLoopIterations)
    : maxRecursionDepth(maxRecursionDepth), maxLoopIterations(maxLoopIterations) {}

void Interpreter::visit(const ProgramNode& node)
{
    for (const auto& decl : node.getDeclarations())
    {
        decl->accept(*this);
    }
    if (functions.find("main") == functions.end())
    {
        throw InterpreterException("missing main function");
    }
    enterCallContext("main");
    functions["main"].block->accept(*this);
}

void Interpreter::visit(const FunctionDeclarationNode& node)
{
    if (functions.find(node.getIdentifier()) != functions.end())
    {
        throw InterpreterException("duplicate function declaration: " + node.getIdentifier());
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

    if (node.getType() == "string")
    {
        value = std::string("");
    }

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
    checkDuplicateId(node.getIdentifier());

    node.getVariant()->accept(*this);

    size_t activeIndex = -1;
    std::vector<Value> values;
    values.resize(parameterTypes.size());

    currentScope().variants[node.getIdentifier()] = std::make_unique<Variant>(parameterTypes, activeIndex, values);
    parameterTypes.clear();
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
        size_t callSizeBefore = callStack.size();
        stmt->accept(*this);
        if (callStack.size() < callSizeBefore)
        {
            break;
        }
        currentScope();
    }
}

void Interpreter::visit(const AssignmentNode& node)
{
    node.getFieldOrFunCall()->accept(*this);
    auto target = toAssign;

    if (auto expression = node.getExpression())
    {
        valueStack.pop();

        expression->accept(*this);
        auto newValue = valueStack.top();
        valueStack.pop();

        if (std::holds_alternative<Variable*>(target))
        {
            Variable* variable = std::get<Variable*>(target);
            if (!variable->isMutable)
            {
                throw InterpreterException("cannot modify a const value");
            }
            variable->value = castValueType(newValue, variable->type);
        }
        else if (std::holds_alternative<std::pair<StructureInstance*, std::string>>(target))
        {
            auto [structInstance, fieldName] = std::get<std::pair<StructureInstance*, std::string>>(target);

            Structure& structure = getStructure(structInstance->type);

            auto fieldIt = std::find_if(structure.fields.begin(), structure.fields.end(),
                [&fieldName](const Field& field) { return field.identifier == fieldName; });

            if (fieldIt == structure.fields.end())
            {
                throw InterpreterException("field '" + fieldName + "' not found in structure '" + structInstance->type + "'");
            }

            if (std::holds_alternative<int>(structInstance->values[fieldName]) ||
                std::holds_alternative<float>(structInstance->values[fieldName]) ||
                std::holds_alternative<std::string>(structInstance->values[fieldName]) ||
                std::holds_alternative<bool>(structInstance->values[fieldName]))
            {
                if (!fieldIt->isMutable)
                {
                    throw InterpreterException("cannot modify a const value");
                }
                std::string type = determineValueType(structInstance->values[fieldName]);
                structInstance->values[fieldName] = castValueType(newValue, type);
            }
            else
            {
                structInstance->values[fieldName] = newValue;
            }
        }
        else if (std::holds_alternative<Variant*>(target))
        {
            Variant* variant = std::get<Variant*>(target);
            
            std::string newType = determineValueType(newValue);

            auto it = std::find(variant->allowedTypes.begin(), variant->allowedTypes.end(), newType);
            if (it != variant->allowedTypes.end())
            {
                size_t index = std::distance(variant->allowedTypes.begin(), it);
                variant->activeIndex = index;
                variant->values[index] = newValue;
            }
            else
            {
                throw InterpreterException("type " + newType + " is not allowed in variant.");
            }
        }

    }
    toAssign = std::monostate{};
}

void Interpreter::visit(const IfStatementNode& node)
{
    node.getCondition()->accept(*this);

    bool condition = std::get<bool>(castValueType(valueStack.top(), "bool"));
    valueStack.pop();

    size_t callSizeBefore = callStack.size();

    enterScope();
    if (condition)
    {
        node.getBlock()->accept(*this);
    }
    else
    {
        node.getElseBlock()->accept(*this);
    }
    if (callStack.size() == callSizeBefore)
    {
        exitScope();
    }
}

void Interpreter::visit(const WhileStatementNode& node)
{
    node.getCondition()->accept(*this);

    bool condition = std::get<bool>(castValueType(valueStack.top(), "bool"));
    valueStack.pop();
    size_t iterationCount = 0;
    size_t callSizeBefore = callStack.size();

    while (condition)
    {
        if (++iterationCount > maxLoopIterations)
        {
            throw InterpreterException("maximum while loop iterations");
        }

        enterScope();
        node.getBlock()->accept(*this);
        if (callStack.size() == callSizeBefore)
        {
            exitScope();
        }
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
        throw InterpreterException("cannot return a value in void function");
    }
    
    node.getExpression()->accept(*this);

    auto value = valueStack.top();
    valueStack.pop();
    value = castValueType(value, functions[functionIdentifier].type);
    valueStack.push(value);

    exitCallContext();
}

void Interpreter::visit(const MatchStatementNode& node)
{
    auto expression = node.getExpression();
    expression->accept(*this);

    if (!std::holds_alternative<std::shared_ptr<Variant>>(valueStack.top()))
    {
        throw InterpreterException("invalid type for match");
    }

    if (std::get<std::shared_ptr<Variant>>(valueStack.top())->activeIndex == -1)
    {
        throw InterpreterException("variant does not hold any value");
    }

    currentScope().typeMatched = false;
    const MatchCaseNode* defaultCase = nullptr;

    for (const auto& matchCase : node.getCases())
    {
        if (std::holds_alternative<std::string>(matchCase->getType()) &&
            std::get<std::string>(matchCase->getType()) == "default")
        {
            if (defaultCase != nullptr)
            {
                throw InterpreterException("duplicate default case");
            }

            defaultCase = matchCase.get();
            continue;
        }

        matchCase->accept(*this);

        if (currentScope().typeMatched)
        {
            break;
        }
    }

    if (!currentScope().typeMatched && defaultCase)
    {
        defaultCase->accept(*this);
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
        std::string variantType = "variant[";
        size_t initialSize = parameterTypes.size();
        std::get<std::unique_ptr<VariantNode>>(node.getType()).get()->accept(*this);
        for (size_t j = initialSize; j < parameterTypes.size(); ++j)
        {
            if (j > initialSize)
            {
                variantType += ", ";
            }
            variantType += parameterTypes[j];
        }

        variantType += "]";
        parameterTypes.resize(initialSize);
        parameterTypes.push_back(variantType); 
    }
}

void Interpreter::visit(const ExpressionNode& node)
{
    node.getLeft()->accept(*this);

    auto a = valueStack.top();
    valueStack.pop();

    for (const auto& right : node.getRights())
    {
        if (std::get<bool>(castValueType(a, "bool")) == true)
        {
            break;
        }
        right->accept(*this);

        auto b = valueStack.top();
        valueStack.pop();

        a = std::get<bool>(castValueType(a, "bool")) || std::get<bool>(castValueType(b, "bool"));
    }
    valueStack.push(a);
}

void Interpreter::visit(const AndExpressionNode& node)
{
    node.getLeft()->accept(*this);

    auto a = valueStack.top();
    valueStack.pop();

    for (const auto& right : node.getRights())
    {
        if (std::get<bool>(castValueType(a, "bool")) == false)
        {
            break;
        }
        right->accept(*this);

        auto b = valueStack.top();
        valueStack.pop();

        a = std::get<bool>(castValueType(a, "bool")) && std::get<bool>(castValueType(b, "bool"));
    }
    valueStack.push(a);
}

void Interpreter::visit(const ComparisonNode& node)
{
    node.getLeft()->accept(*this);

    auto a = valueStack.top();
    valueStack.pop();

    ComparisonOperator op = node.getOp();

    if (op != ComparisonOperator::NONE)
    {
        node.getRight()->accept(*this);

        auto b = valueStack.top();
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
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " == " + determineValueType(b));
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
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " != " + determineValueType(b));
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
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " > " + determineValueType(b));
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
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " >= " + determineValueType(b));
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
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " < " + determineValueType(b));
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
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " <= " + determineValueType(b));
                }
                break;
            }
        }
    }
    valueStack.push(a);
}

void Interpreter::visit(const AddExpressionNode& node)
{
    node.getLeft()->accept(*this);

    auto a = valueStack.top();
    valueStack.pop();

    for (const auto& pair : node.getRights())
    {
        AddOperator op = pair.first;

        pair.second.get()->accept(*this);

        auto b = valueStack.top();
        valueStack.pop();

        switch (op)
        {
            case AddOperator::PLUS:
            {
                if (std::holds_alternative<int>(a))
                {
                    int aInt = std::get<int>(a);
                    int bInt = std::get<int>(castValueType(b, "int"));
                    
                    if ((bInt > 0 && aInt > std::numeric_limits<int>::max() - bInt) ||
                        (bInt < 0 && aInt < std::numeric_limits<int>::min() - bInt))
                    {
                        throw InterpreterException("integer overflow");
                    }
                    a = aInt + bInt;
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) + std::get<float>(castValueType(b, "float"));
                    if (std::isinf(std::get<float>(a)))
                    {
                        throw InterpreterException("float overflow");
                    }
                }
                else if (std::holds_alternative<std::string>(a))
                {
                    a = std::get<std::string>(a) + std::get<std::string>(castValueType(b, "string"));
                }
                else
                {
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " + " + determineValueType(b));
                }
                break;
            }
            case AddOperator::MINUS:
            {
                if (std::holds_alternative<int>(a))
                {
                    int aInt = std::get<int>(a);
                    int bInt = std::get<int>(castValueType(b, "int"));
                    
                    if ((bInt > 0 && aInt < std::numeric_limits<int>::min() + bInt) ||
                        (bInt < 0 && aInt > std::numeric_limits<int>::max() + bInt))
                    {
                        throw InterpreterException("integer overflow");
                    }
                    a = aInt - bInt;
                }
                else if (std::holds_alternative<float>(a))
                {
                    a = std::get<float>(a) - std::get<float>(castValueType(b, "float"));
                    if (std::isinf(std::get<float>(a)))
                    {
                        throw InterpreterException("float overflow");
                    }
                }
                else
                {
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " - " + determineValueType(b));
                }
                break;
            }
        }
    }
    valueStack.push(a);
}

void Interpreter::visit(const MultExpressionNode& node)
{
    node.getLeft()->accept(*this);

    auto a = valueStack.top();
    valueStack.pop();

    for (const auto& pair : node.getRights())
    {
        MultOperator op = pair.first;

        pair.second.get()->accept(*this);

        auto b = valueStack.top();
        valueStack.pop();

        switch (op)
        {
            case MultOperator::ASTERISK:
            {
                if (std::holds_alternative<float>(a) || std::holds_alternative<float>(b))
                {
                    a = std::get<float>(castValueType(a, "float")) * std::get<float>(castValueType(b, "float"));
                    if (std::isinf(std::get<float>(a)))
                    {
                        throw InterpreterException("float overflow");
                    }
                }
                else if (std::holds_alternative<int>(a))
                {
                    int aInt = std::get<int>(a);
                    int bInt = std::get<int>(castValueType(b, "int"));

                    if (bInt != 0)
                    {
                        if (aInt > std::numeric_limits<int>::max() / bInt ||
                            aInt < std::numeric_limits<int>::min() / bInt)
                        {
                            throw InterpreterException("integer overflow");
                        }
                    }
                    a = aInt * bInt;
                }
                else
                {
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " * " + determineValueType(b));
                }
                break;
            }
            case MultOperator::SLASH:
            {
                if (std::holds_alternative<float>(a) || std::holds_alternative<float>(b))
                {
                    if (std::get<float>(castValueType(b, "float")) == 0)
                    {
                        throw InterpreterException("division by zero: " + std::get<std::string>(castValueType(a, "string")) +
                                                    " / " + std::get<std::string>(castValueType(b, "string")));
                    }
                    a = std::get<float>(castValueType(a, "float")) / std::get<float>(castValueType(b, "float"));
                    if (std::isinf(std::get<float>(a)))
                    {
                        throw InterpreterException("float overflow");
                    }
                }
                else if (std::holds_alternative<int>(a))
                {
                    int aInt = std::get<int>(a);
                    int bInt = std::get<int>(castValueType(b, "int"));
                    if (bInt == 0)
                    {
                        throw InterpreterException("division by zero: " + std::get<std::string>(castValueType(a, "string")) +
                                                    " / " + std::get<std::string>(castValueType(b, "string")));
                    }
                    a = aInt / bInt;
                }
                else
                {
                    throw InterpreterException("invalid types for operation: " + determineValueType(a) + " / " + determineValueType(b));
                }
                break;
            }
        }
    }
    valueStack.push(a);
}

void Interpreter::visit(const TermNode& node)
{
    NegationType negation = node.getNegation();

    node.getContent()->accept(*this);

    auto value = valueStack.top();
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
                throw InterpreterException("unsupported type for this negation type: !" + determineValueType(value));
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
                throw InterpreterException("unsupported type for this negation type: -" + determineValueType(value));
            }
            break;
        }
    }

    valueStack.push(value);
}

void Interpreter::visit(const FieldOrFunCallNode& node)
{
    std::string identifier = node.getIdentifier();

    if (identifier == "print")
    {
        if (!std::holds_alternative<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()))
        {
            throw InterpreterException("calling print requires an argument");
        }

        auto arguments = std::get<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()).get();
        size_t stackSizeBefore = valueStack.size();
        arguments->accept(*this);
        size_t argumentCount = valueStack.size() - stackSizeBefore;

        if (argumentCount != 1)
        {
            throw InterpreterException("function " + identifier +
                                        " requires 1 argument(s), but " + std::to_string(argumentCount) +
                                        " were given");
        }

        auto value = valueStack.top();
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
            if (std::get<bool>(value))
            {
                std::cout << "true";
            }
            else
            {
                std::cout << "false";
            }
        }
        return;
    }

    if (std::holds_alternative<std::vector<std::string>>(node.getAdditionalContent()))
    {
        auto fields = std::get<std::vector<std::string>>(node.getAdditionalContent());

        StructureInstance* structInstance = &getStructureInstance(identifier);

        for (size_t i = 0; i < fields.size() - 1; ++i)
        {
            if (!structInstance->values.count(fields[i]))
            {
                throw InterpreterException("field not found: " + fields[i]);
            }

            if (!std::holds_alternative<std::shared_ptr<StructureInstance>>(structInstance->values[fields[i]]))
            {
                throw InterpreterException("attempted field access on non-struct field: " + fields[i]);
            }

            structInstance = std::get<std::shared_ptr<StructureInstance>>(structInstance->values[fields[i]]).get();
        }

        toAssign = std::make_pair(structInstance, fields.back());
        if (!structInstance->values.count(fields.back()))
        {
            throw InterpreterException("field not found: " + fields.back());
        }
        valueStack.push(structInstance->values[fields.back()]);
    }
    else if (std::holds_alternative<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()))
    {
        auto arguments = std::get<std::unique_ptr<ArgumentListNode>>(node.getAdditionalContent()).get();
        size_t stackSizeBefore = valueStack.size();
        arguments->accept(*this);
        size_t argumentCount = valueStack.size() - stackSizeBefore;

        const Function& function = getFunction(identifier);

        if (argumentCount != function.parameters.size())
        {
            throw InterpreterException("function " + identifier +
                                        " requires " + std::to_string(function.parameters.size()) +
                                        " argument(s), but " + std::to_string(argumentCount) +
                                        " were given");
        }

        enterCallContext(identifier);
        for (auto i = function.parameters.size(); i > 0; --i)
        {
            const auto& param = function.parameters[i-1];
            auto value = valueStack.top();
            valueStack.pop();

            if (determineValueType(value) != param.first)
            {
                throw InterpreterException("invalid argument type\nFound: " +
                                            determineValueType(value) + "\nExpected: " + 
                                            param.first);
            }

            if (std::holds_alternative<std::shared_ptr<Variant>>(value))
            {
                currentScope().variants[param.second] = std::get<std::shared_ptr<Variant>>(value);
            }
            else
            {
                currentScope().variables[param.second] = Variable{true, param.first, value};
            }
        }
        size_t callSizeBefore = callStack.size();
        function.block->accept(*this);
        if (callStack.size() == callSizeBefore)
        {
            exitCallContext();
        }
    }
    else
    {
        try
        {
            std::shared_ptr<Variant>& variant = getVariant(identifier);
            valueStack.push(variant);
            toAssign = variant.get();
        }
        catch(const InterpreterException& e)
        {
            try
            {
                StructureInstance& structInstance = getStructureInstance(identifier);
                valueStack.push(std::make_shared<StructureInstance>(structInstance));
            }
            catch(const std::exception& e)
            {
                Variable& variable = getVariable(identifier);
                valueStack.push(variable.value);
                toAssign = &variable;
            }
        }
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
    size_t stackSizeBefore = valueStack.size();
    arguments->accept(*this);
    size_t argumentCount = valueStack.size() - stackSizeBefore;

    Structure& structure = getStructure(type);

    if (argumentCount != structure.fields.size())
    {
        throw InterpreterException("structure " + identifier +
                                    " requires " + std::to_string(structure.fields.size()) +
                                    " argument(s), but " + std::to_string(argumentCount) +
                                    " were given");
    }

    StructureInstance structInstance;
    structInstance.type = type;

    for (auto i = structure.fields.size(); i > 0; --i)
    {
        const auto& field = structure.fields[i - 1];
        auto value = valueStack.top();
        valueStack.pop();

        if (determineValueType(value) != field.type)
        {
            throw InterpreterException("invalid argument type for field " + field.identifier + 
                                        " in structure " + type + 
                                        "\nFound: " + determineValueType(value) + 
                                        "\nExpected: " + field.type);
        }

        structInstance.values[field.identifier] = value;
    }

    currentScope().structInstances[identifier] = structInstance;
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
        auto variantDeclaration = std::get<std::unique_ptr<VariantDeclarationNode>>(node.getContent()).get();
        variantDeclaration->accept(*this);

        identifier = variantDeclaration->getIdentifier();
        type = buildVariantType(*currentScope().variants[identifier]);
        
        currentScope().variants.erase(identifier);
    }
    if (isMutable && type != "int" && type != "float" && type != "string" && type != "bool")
    {
        throw InterpreterException("cannot use 'mut' to this type of field: " + type);
    }
    fieldBuffer.push_back(Field{isMutable, type, identifier});
}

void Interpreter::visit(const VariantNode& node)
{
    for (size_t i = 0; i < node.getTypes().size(); ++i)
    {
        const auto& type = node.getTypes()[i];

        if (std::holds_alternative<std::string>(type))
        {
            parameterTypes.push_back(std::get<std::string>(type));
        }
        else if (std::holds_alternative<std::unique_ptr<VariantNode>>(type))
        {
            std::string variantType = "variant[";
            size_t initialSize = parameterTypes.size();

            auto variant = std::get<std::unique_ptr<VariantNode>>(type).get();
            variant->accept(*this);

            for (size_t j = initialSize; j < parameterTypes.size(); ++j)
            {
                if (j > initialSize)
                {
                    variantType += ", ";
                }
                variantType += parameterTypes[j];
            }

            variantType += "]";
            parameterTypes.resize(initialSize);
            parameterTypes.push_back(variantType);
        }
    }
}

void Interpreter::visit(const MatchCaseNode& node)
{
    if (std::holds_alternative<std::string>(node.getType()) &&
        std::get<std::string>(node.getType()) == "default")
    {
        enterScope();
        node.getBlock()->accept(*this);
        exitScope();
        return;
    }

    auto value = valueStack.top();
    valueStack.pop();

    std::string type;
    if (std::holds_alternative<std::string>(node.getType()))
    {
        type = std::get<std::string>(node.getType());
    }
    else if (std::holds_alternative<std::unique_ptr<VariantNode>>(node.getType()))
    {
        type = "variant[";
        size_t initialSize = parameterTypes.size();
        auto variant = std::get<std::unique_ptr<VariantNode>>(node.getType()).get();
        variant->accept(*this);
        for (size_t j = initialSize; j < parameterTypes.size(); ++j)
        {
            if (j > initialSize)
            {
                type += ", ";
            }
            type += parameterTypes[j];
        }

        type += "]";
        parameterTypes.resize(initialSize);
    }

    std::string identifier = node.getIdentifier();
    size_t activeIndex = std::get<std::shared_ptr<Variant>>(value)->activeIndex;

    if (type == std::get<std::shared_ptr<Variant>>(value)->allowedTypes[activeIndex])
    {
        currentScope().typeMatched = true;
        enterScope();
        auto storedValue = std::get<std::shared_ptr<Variant>>(value)->values[activeIndex];
        if (std::holds_alternative<int>(storedValue) ||
            std::holds_alternative<float>(storedValue) ||
            std::holds_alternative<std::string>(storedValue) ||
            std::holds_alternative<bool>(storedValue))
        {
            currentScope().variables[identifier] = Variable{false, type, storedValue};
        }
        else if (std::holds_alternative<std::shared_ptr<Variant>>(storedValue))
        {
            auto& nestedVariant = std::get<std::shared_ptr<Variant>>(storedValue);
            currentScope().variants[identifier] = nestedVariant;
        }
        node.getBlock()->accept(*this);
        exitScope();
    }
    else
    {
        valueStack.push(value);
    }
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